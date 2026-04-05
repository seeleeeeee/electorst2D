#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QActionGroup>
#include <QDebug>
#include <QWheelEvent>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QTableWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug() << "=== MAINWINDOW START ===";
    
    // Создаем сцену
    scene = new GeometryScene(this);
    ui->graphicsView->setScene(scene);
    
    connect(scene, &GeometryScene::statusMessage, this, &MainWindow::updateStatusBar);
    
    scene->setSceneRect(-500, -500, 1000, 1000);
    scene->setBackgroundBrush(Qt::white);
    
    // Настройка группы действий
    QActionGroup *drawGroup = new QActionGroup(this);
    drawGroup->addAction(ui->actionAddNode);
    drawGroup->addAction(ui->actionAddEdge);
    drawGroup->addAction(ui->actionSelectGroup);
    drawGroup->setExclusive(true);
    
    ui->actionAddEdge->setChecked(true);
    scene->setMode(GeometryScene::AddEdgeMode);
    
    // Настройка панели материалов
    ui->treeMaterials->setColumnCount(1);
    ui->treeMaterials->setHeaderLabel("Материалы");
    
    // Добавляем диэлектрики
    QTreeWidgetItem *dielectrics = new QTreeWidgetItem(ui->treeMaterials);
    dielectrics->setText(0, "Диэлектрики");
    for (const auto& d : electrostatic::DIELECTRICS) {
        QTreeWidgetItem *item = new QTreeWidgetItem(dielectrics);
        item->setText(0, QString("%1 (ε=%2)").arg(QString::fromStdString(d.name)).arg(d.erReal));
    }
    dielectrics->setExpanded(true);
    
    // Добавляем проводники
    QTreeWidgetItem *conductors = new QTreeWidgetItem(ui->treeMaterials);
    conductors->setText(0, "Проводники");
    for (const auto& c : electrostatic::CONDUCTORS) {
        QTreeWidgetItem *item = new QTreeWidgetItem(conductors);
        item->setText(0, QString("%1 (σ=%2e6)").arg(QString::fromStdString(c.name)).arg(c.conductivity/1e6, 0, 'f', 2));
    }
    conductors->setExpanded(true);
    
    // Настройка граничных условий
    ui->tableBoundary->setHorizontalHeaderLabels({"Граница", "Напряжение (В)"});
    ui->tableBoundary->setRowCount(2);
    ui->tableBoundary->setItem(0, 0, new QTableWidgetItem("Нижняя"));
    ui->tableBoundary->setItem(0, 1, new QTableWidgetItem("0"));
    ui->tableBoundary->setItem(1, 0, new QTableWidgetItem("Верхняя"));
    ui->tableBoundary->setItem(1, 1, new QTableWidgetItem("10"));
    
    // Подключаем кнопки
    connect(ui->btnBuildMesh, &QPushButton::clicked, this, &MainWindow::on_actionBuildMesh_triggered);
    connect(ui->btnAnalyze, &QPushButton::clicked, this, &MainWindow::on_actionAnalyze_triggered);
    
    // Инициализация визуализатора
    visualizer = std::make_unique<electrostatic::QtVisualizer>(scene);
    
    updateStatusBar("Готов. N - точки, L - линии, колесо мыши - зум");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    double factor = 1.1;
    if (event->angleDelta().y() > 0) {
        ui->graphicsView->scale(factor, factor);
    } else {
        ui->graphicsView->scale(1.0 / factor, 1.0 / factor);
    }
    event->accept();
}

void MainWindow::updateStatusBar(const QString& msg)
{
    statusBar()->showMessage(msg, 2000);
    qDebug() << "Status:" << msg;
}

// Файл
void MainWindow::on_actionNew_triggered()
{
    if (scene) scene->clearAll();
    currentMesh.reset();
    if (visualizer) visualizer->clear();
    updateStatusBar("Новый проект");
}

void MainWindow::on_actionOpen_triggered()
{
    updateStatusBar("Открыть проект");
}

void MainWindow::on_actionSave_triggered()
{
    updateStatusBar("Сохранить проект");
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

// Вид
void MainWindow::on_actionZoomIn_triggered()
{
    ui->graphicsView->scale(1.2, 1.2);
}

void MainWindow::on_actionZoomOut_triggered()
{
    ui->graphicsView->scale(0.8, 0.8);
}

void MainWindow::on_actionFitView_triggered()
{
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::on_actionShowGrid_toggled(bool checked)
{
    if (scene) scene->setGridEnabled(checked);
    updateStatusBar(checked ? "Сетка включена" : "Сетка выключена");
}

// Инструменты
void MainWindow::on_actionAddNode_triggered()
{
    if (scene) scene->setMode(GeometryScene::AddNodeMode);
}

void MainWindow::on_actionAddEdge_triggered()
{
    if (scene) scene->setMode(GeometryScene::AddEdgeMode);
}

void MainWindow::on_actionSelectGroup_triggered()
{
    if (scene) scene->setMode(GeometryScene::SelectMode);
}

void MainWindow::on_actionClearAll_triggered()
{
    if (scene) scene->clearAll();
    if (visualizer) visualizer->clear();
    currentMesh.reset();
    updateStatusBar("Все очищено");
}

// Генерация сетки ТОЛЬКО ВНУТРИ ПОЛИГОНА
void MainWindow::on_actionBuildMesh_triggered()
{
    // Получаем полигон из линий
    QVector<QPointF> polygon = scene->getPolygon();
    
    if (polygon.size() < 3) {
        updateStatusBar("Ошибка: нужно минимум 3 точки, образующие замкнутый контур!");
        QMessageBox::warning(this, "Ошибка", 
            "Нет замкнутого контура!\n\n"
            "1. Нажмите N и создайте 3 точки\n"
            "2. Нажмите L и соедините точки по кругу:\n"
            "   - точка1 → точка2\n"
            "   - точка2 → точка3\n"
            "   - точка3 → точка1\n"
            "3. Нажмите Ctrl+M для построения сетки");
        return;
    }
    
    updateStatusBar(QString("Генерация сетки внутри полигона из %1 точек...").arg(polygon.size()));
    
    // Функция проверки точки внутри полигона (алгоритм луча)
    auto pointInPolygon = [&polygon](double x, double y) -> bool {
        bool inside = false;
        int n = polygon.size();
        for (int i = 0, j = n - 1; i < n; j = i++) {
            double xi = polygon[i].x(), yi = polygon[i].y();
            double xj = polygon[j].x(), yj = polygon[j].y();
            bool intersect = ((yi > y) != (yj > y)) &&
                             (x < (xj - xi) * (y - yi) / (yj - yi) + xi);
            if (intersect) inside = !inside;
        }
        return inside;
    };
    
    // Находим границы полигона
    double minX = polygon[0].x(), maxX = polygon[0].x();
    double minY = polygon[0].y(), maxY = polygon[0].y();
    for (const auto& p : polygon) {
        if (p.x() < minX) minX = p.x();
        if (p.x() > maxX) maxX = p.x();
        if (p.y() < minY) minY = p.y();
        if (p.y() > maxY) maxY = p.y();
    }
    
    // Добавляем отступ
    double padding = 20;
    minX -= padding;
    maxX += padding;
    minY -= padding;
    maxY += padding;
    
    // Размер сетки
    int resolution = ui->spinMeshSize->value() * 20;
    if (resolution < 10) resolution = 10;
    if (resolution > 80) resolution = 80;
    
    // Создаем сетку
    currentMesh = std::make_shared<electrostatic::Mesh>();
    std::vector<std::vector<int>> nodeIndex(resolution, std::vector<int>(resolution, -1));
    
    // Сначала создаем все узлы внутри полигона
    int nodeId = 0;
    for (int i = 0; i < resolution; i++) {
        for (int j = 0; j < resolution; j++) {
            double x = minX + (maxX - minX) * i / (resolution - 1);
            double y = minY + (maxY - minY) * j / (resolution - 1);
            
            if (pointInPolygon(x, y)) {
                currentMesh->nodes.push_back(electrostatic::Node(nodeId, x, y));
                nodeIndex[i][j] = nodeId;
                nodeId++;
            }
        }
    }
    
    qDebug() << "Узлов внутри полигона:" << currentMesh->nodes.size();
    
    if (currentMesh->nodes.empty()) {
        updateStatusBar("Ошибка: нет узлов внутри полигона!");
        QMessageBox::warning(this, "Ошибка", "Полигон слишком маленький или неправильный!");
        return;
    }
    
    // Создаем треугольники
    int elemId = 0;
    for (int i = 0; i < resolution - 1; i++) {
        for (int j = 0; j < resolution - 1; j++) {
            int n0 = nodeIndex[i][j];
            int n1 = nodeIndex[i+1][j];
            int n2 = nodeIndex[i][j+1];
            int n3 = nodeIndex[i+1][j+1];
            
            // Первый треугольник
            if (n0 != -1 && n1 != -1 && n2 != -1) {
                currentMesh->elements.push_back(electrostatic::Element(elemId++, 1, {n0, n1, n2}));
            }
            // Второй треугольник
            if (n1 != -1 && n3 != -1 && n2 != -1) {
                currentMesh->elements.push_back(electrostatic::Element(elemId++, 1, {n1, n3, n2}));
            }
        }
    }
    
    qDebug() << "Элементов:" << currentMesh->elements.size();
    
    // Визуализируем сетку
    if (visualizer) {
        visualizer->clear();
        visualizer->drawMesh(currentMesh);
    }
    
    updateStatusBar(QString("Сетка: %1 узлов, %2 элементов внутри полигона")
        .arg(currentMesh->nodes.size())
        .arg(currentMesh->elements.size()));
}

void MainWindow::on_actionAnalyze_triggered()
{
    if (!currentMesh || currentMesh->nodes.empty()) {
        updateStatusBar("Сначала постройте сетку!");
        QMessageBox::warning(this, "Ошибка", "Сначала постройте сетку (Ctrl+M)!");
        return;
    }
    
    updateStatusBar("Решение...");
    
    // Получаем граничные условия из таблицы
    std::vector<electrostatic::BoundaryCondition> bc;
    for (int i = 0; i < ui->tableBoundary->rowCount(); i++) {
        QTableWidgetItem* nameItem = ui->tableBoundary->item(i, 0);
        QTableWidgetItem* valItem = ui->tableBoundary->item(i, 1);
        if (nameItem && valItem && !nameItem->text().isEmpty()) {
            bc.push_back({nameItem->text().toStdString(), 
                         electrostatic::BoundaryType::Dirichlet, 
                         valItem->text().toDouble(), 
                         nameItem->text().toStdString()});
        }
    }
    
    // Создаем решение
    currentSolution.nodes = currentMesh->nodes;
    currentSolution.elements = currentMesh->elements;
    currentSolution.energy = 0;
    
    // Находим границы полигона для линейного потенциала
    double minY = 1e9, maxY = -1e9;
    for (const auto& node : currentSolution.nodes) {
        if (node.y < minY) minY = node.y;
        if (node.y > maxY) maxY = node.y;
    }
    
    // Линейный потенциал
    for (auto& node : currentSolution.nodes) {
        node.potential = (node.y - minY) / (maxY - minY) * 10.0;
        if (node.potential < 0) node.potential = 0;
        if (node.potential > 10) node.potential = 10;
    }
    
    // Применяем граничные условия
    for (const auto& cond : bc) {
        if (cond.name == "Нижняя") {
            for (auto& node : currentSolution.nodes) {
                if (node.y < minY + 10) node.potential = cond.value;
            }
        } else if (cond.name == "Верхняя") {
            for (auto& node : currentSolution.nodes) {
                if (node.y > maxY - 10) node.potential = cond.value;
            }
        }
    }
    
    // Визуализируем потенциал
    if (visualizer) {
        electrostatic::ColorMap cmap;
        cmap.minValue = 0;
        cmap.maxValue = 10;
        visualizer->drawPotential(
            std::make_shared<electrostatic::Solution>(currentSolution), 
            cmap
        );
    }
    
    updateStatusBar(QString("Готово! Энергия: %1 Дж").arg(currentSolution.energy));
}

void MainWindow::on_actionMaterials_triggered()
{
    updateStatusBar("Материалы");
}

void MainWindow::on_actionSettings_triggered()
{
    updateStatusBar("Настройки");
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "О программе",
        "Electrostatic2D v1.0\n"
        "Расчет электростатических полей методом конечных элементов\n"
        "© 2026\n\n"
        "Управление:\n"
        "N - точки, L - линии, колесо мыши - зум\n"
        "Ctrl+M - построить сетку внутри фигуры\n"
        "Ctrl+R - анализ\n\n"
        "Создание фигуры:\n"
        "1. Нажмите N, создайте 3 точки\n"
        "2. Нажмите L, соедините точки по кругу\n"
        "3. Нажмите Ctrl+M для сетки внутри фигуры");
}

void MainWindow::onSettingsChanged(int width, int height, double gridSize)
{
    scene->setSceneRect(-width/2, -height/2, width, height);
    if (scene) scene->setGridSize(gridSize);
}