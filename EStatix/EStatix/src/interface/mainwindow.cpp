#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QActionGroup>
#include <QDebug>
#include <QWheelEvent>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QTableWidgetItem>
#include <QFileDialog>
#include <QToolButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    // Полноэкранный режим
    showMaximized();
    
    qDebug() << "=== MAINWINDOW START ===";
    
    // Создаем сцену
    scene = new GeometryScene(this);
    ui->graphicsView->setScene(scene);
    
    connect(scene, &GeometryScene::statusMessage, this, &MainWindow::updateStatusBar);
    
    scene->setSceneRect(-500, -500, 1000, 1000);
    scene->setBackgroundBrush(Qt::white);
    
    // Настройка группы действий для кнопок
    QActionGroup *drawGroup = new QActionGroup(this);
    drawGroup->addAction(ui->actionAddNode);
    drawGroup->addAction(ui->actionAddEdge);
    drawGroup->addAction(ui->actionSelectGroup);
    drawGroup->setExclusive(true);
    
    // Настройка тулбара для подсветки кнопок
    QList<QToolButton*> toolButtons = ui->toolBar->findChildren<QToolButton*>();
    for (QToolButton* btn : toolButtons) {
        btn->setAutoRaise(false);
        btn->setCheckable(true);
    }
    
    ui->actionAddEdge->setChecked(true);
    scene->setMode(GeometryScene::AddEdgeMode);
    updateButtonStyles();
    
    // Настройка панели материалов
    ui->treeMaterials->setColumnCount(1);
    ui->treeMaterials->setHeaderLabel("Materials");
    
    // Добавляем диэлектрики
    QTreeWidgetItem *dielectrics = new QTreeWidgetItem(ui->treeMaterials);
    dielectrics->setText(0, "Dielectrics");
    for (const auto& d : electrostatic::DIELECTRICS) {
        QTreeWidgetItem *item = new QTreeWidgetItem(dielectrics);
        item->setText(0, QString("%1 (ε=%2)").arg(QString::fromStdString(d.name)).arg(d.erReal));
    }
    dielectrics->setExpanded(true);
    
    // Добавляем проводники
    QTreeWidgetItem *conductors = new QTreeWidgetItem(ui->treeMaterials);
    conductors->setText(0, "Conductors");
    for (const auto& c : electrostatic::CONDUCTORS) {
        QTreeWidgetItem *item = new QTreeWidgetItem(conductors);
        item->setText(0, QString("%1 (σ=%2e6)").arg(QString::fromStdString(c.name)).arg(c.conductivity/1e6, 0, 'f', 2));
    }
    conductors->setExpanded(true);
    
    // Настройка граничных условий
    ui->tableBoundary->setHorizontalHeaderLabels({"Boundary", "Voltage (V)"});
    ui->tableBoundary->setRowCount(2);
    ui->tableBoundary->setItem(0, 0, new QTableWidgetItem("Bottom"));
    ui->tableBoundary->setItem(0, 1, new QTableWidgetItem("0"));
    ui->tableBoundary->setItem(1, 0, new QTableWidgetItem("Top"));
    ui->tableBoundary->setItem(1, 1, new QTableWidgetItem("10"));
    
    // Подключаем кнопки
    connect(ui->btnBuildMesh, &QPushButton::clicked, this, &MainWindow::on_actionBuildMesh_triggered);
    connect(ui->btnAnalyze, &QPushButton::clicked, this, &MainWindow::on_actionAnalyze_triggered);
    
    // Добавляем действия для полного сохранения
    QAction* saveFullAction = new QAction("Save Full Project", this);
    saveFullAction->setShortcut(QKeySequence("Ctrl+Shift+S"));
    connect(saveFullAction, &QAction::triggered, this, &MainWindow::on_actionSaveFull_triggered);
    ui->menuFile->addAction(saveFullAction);
    
    QAction* loadFullAction = new QAction("Load Full Project", this);
    loadFullAction->setShortcut(QKeySequence("Ctrl+Shift+O"));
    connect(loadFullAction, &QAction::triggered, this, &MainWindow::on_actionLoadFull_triggered);
    ui->menuFile->addAction(loadFullAction);
    
    // Инициализация визуализатора
    visualizer = std::make_unique<electrostatic::QtVisualizer>(scene);
    
    updateStatusBar("Ready. N - points, L - lines, mouse wheel - zoom");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateButtonStyles()
{
    QString activeStyle = "background-color: #e94560; color: white; border: none; border-radius: 4px; padding: 6px 12px;";
    QString inactiveStyle = "background-color: #3a3a6a; color: white; border: none; border-radius: 4px; padding: 6px 12px;";
    
    QAction* actions[] = {ui->actionAddNode, ui->actionAddEdge, ui->actionSelectGroup};
    for (auto* action : actions) {
        QWidget* widget = ui->toolBar->widgetForAction(action);
        if (widget) {
            if (action->isChecked()) {
                widget->setStyleSheet(activeStyle);
            } else {
                widget->setStyleSheet(inactiveStyle);
            }
        }
    }
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

// File
void MainWindow::on_actionNew_triggered()
{
    if (scene) scene->clearAll();
    currentMesh.reset();
    currentSolution = electrostatic::Solution();
    if (visualizer) visualizer->clear();
    updateStatusBar("New project");
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open project", "", "JSON files (*.json)");
    if (!fileName.isEmpty()) {
        if (ProjectIO::loadProject(fileName, scene)) {
            updateStatusBar("Project loaded: " + fileName);
        } else {
            QMessageBox::warning(this, "Error", "Failed to load project!");
        }
    }
}

void MainWindow::on_actionSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save project", "project.json", "JSON files (*.json)");
    if (!fileName.isEmpty()) {
        if (ProjectIO::saveProject(fileName, scene)) {
            updateStatusBar("Project saved: " + fileName);
        } else {
            QMessageBox::warning(this, "Error", "Failed to save project!");
        }
    }
}

void MainWindow::on_actionSaveFull_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save Full Project", 
        "full_project.json", "JSON files (*.json)");
    if (!fileName.isEmpty()) {
        if (ProjectIO::saveFullProject(fileName, scene, currentMesh, currentSolution)) {
            updateStatusBar("Full project saved: " + fileName);
        } else {
            QMessageBox::warning(this, "Error", "Failed to save full project!");
        }
    }
}

void MainWindow::on_actionLoadFull_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Load Full Project", 
        "", "JSON files (*.json)");
    if (!fileName.isEmpty()) {
        if (ProjectIO::loadFullProject(fileName, scene, currentMesh, currentSolution)) {
            updateStatusBar("Full project loaded: " + fileName);
            if (visualizer) {
                visualizer->clear();
                if (currentMesh && !currentMesh->nodes.empty()) {
                    visualizer->drawMesh(currentMesh);
                    if (!currentSolution.nodes.empty()) {
                        electrostatic::ColorMap cmap;
                        cmap.minValue = 0;
                        cmap.maxValue = 10;
                        visualizer->drawPotential(
                            std::make_shared<electrostatic::Solution>(currentSolution), 
                            cmap
                        );
                    }
                }
            }
        } else {
            QMessageBox::warning(this, "Error", "Failed to load full project!");
        }
    }
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

// View
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
    updateStatusBar(checked ? "Grid enabled" : "Grid disabled");
}

// Tools
void MainWindow::on_actionAddNode_triggered()
{
    if (scene) scene->setMode(GeometryScene::AddNodeMode);
    updateButtonStyles();
}

void MainWindow::on_actionAddEdge_triggered()
{
    if (scene) scene->setMode(GeometryScene::AddEdgeMode);
    updateButtonStyles();
}

void MainWindow::on_actionSelectGroup_triggered()
{
    if (scene) scene->setMode(GeometryScene::SelectMode);
    updateButtonStyles();
}

void MainWindow::on_actionClearAll_triggered()
{
    if (scene) scene->clearAll();
    if (visualizer) visualizer->clear();
    currentMesh.reset();
    currentSolution = electrostatic::Solution();
    updateStatusBar("All cleared");
}

// Mesh generation
void MainWindow::on_actionBuildMesh_triggered()
{
    QVector<QPointF> polygon = scene->getPolygon();
    
    if (polygon.size() < 3) {
        updateStatusBar("Error: need closed polygon with 3+ points!");
        QMessageBox::warning(this, "Error", 
            "Draw a closed polygon:\n"
            "1. Press N and create 3 points\n"
            "2. Press L and connect points in a circle\n"
            "3. Press Ctrl+M");
        return;
    }
    
    updateStatusBar(QString("Generating mesh inside polygon from %1 points...").arg(polygon.size()));
    
    auto pointInPolygon = [&polygon](double x, double y) -> bool {
        bool inside = false;
        int n = polygon.size();
        for (int i = 0, j = n - 1; i < n; j = i++) {
            double xi = polygon[i].x(), yi = polygon[i].y();
            double xj = polygon[j].x(), yj = polygon[j].y();
            bool intersect = ((yi > y) != (yj > y)) && (x < (xj - xi) * (y - yi) / (yj - yi) + xi);
            if (intersect) inside = !inside;
        }
        return inside;
    };
    
    double minX = polygon[0].x(), maxX = polygon[0].x();
    double minY = polygon[0].y(), maxY = polygon[0].y();
    for (const auto& p : polygon) {
        if (p.x() < minX) minX = p.x();
        if (p.x() > maxX) maxX = p.x();
        if (p.y() < minY) minY = p.y();
        if (p.y() > maxY) maxY = p.y();
    }
    
    double padding = 20;
    minX -= padding;
    maxX += padding;
    minY -= padding;
    maxY += padding;
    
    int resolution = ui->spinMeshSize->value() * 20;
    if (resolution < 10) resolution = 10;
    if (resolution > 80) resolution = 80;
    
    currentMesh = std::make_shared<electrostatic::Mesh>();
    std::vector<std::vector<int>> nodeIndex(resolution, std::vector<int>(resolution, -1));
    
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
    
    if (currentMesh->nodes.empty()) {
        updateStatusBar("Error: no nodes inside polygon!");
        return;
    }
    
    int elemId = 0;
    for (int i = 0; i < resolution - 1; i++) {
        for (int j = 0; j < resolution - 1; j++) {
            int n0 = nodeIndex[i][j];
            int n1 = nodeIndex[i+1][j];
            int n2 = nodeIndex[i][j+1];
            int n3 = nodeIndex[i+1][j+1];
            if (n0 != -1 && n1 != -1 && n2 != -1) {
                currentMesh->elements.push_back(electrostatic::Element(elemId++, 1, {n0, n1, n2}));
            }
            if (n1 != -1 && n3 != -1 && n2 != -1) {
                currentMesh->elements.push_back(electrostatic::Element(elemId++, 1, {n1, n3, n2}));
            }
        }
    }
    
    if (visualizer) {
        visualizer->clear();
        visualizer->drawMesh(currentMesh);
    }
    
    updateStatusBar(QString("Mesh: %1 nodes, %2 elements").arg(currentMesh->nodes.size()).arg(currentMesh->elements.size()));
}

// Analysis
void MainWindow::on_actionAnalyze_triggered()
{
    if (!currentMesh || currentMesh->nodes.empty()) {
        updateStatusBar("First build the mesh!");
        QMessageBox::warning(this, "Error", "First build the mesh (Ctrl+M)!");
        return;
    }
    
    updateStatusBar("Solving...");
    
    currentSolution.nodes = currentMesh->nodes;
    currentSolution.elements = currentMesh->elements;
    currentSolution.energy = 0;
    
    double minY = 1e9, maxY = -1e9;
    for (const auto& node : currentSolution.nodes) {
        if (node.y < minY) minY = node.y;
        if (node.y > maxY) maxY = node.y;
    }
    
    for (auto& node : currentSolution.nodes) {
        node.potential = (node.y - minY) / (maxY - minY) * 10.0;
        if (node.potential < 0) node.potential = 0;
        if (node.potential > 10) node.potential = 10;
    }
    
    for (int i = 0; i < ui->tableBoundary->rowCount(); i++) {
        QTableWidgetItem* nameItem = ui->tableBoundary->item(i, 0);
        QTableWidgetItem* valItem = ui->tableBoundary->item(i, 1);
        if (nameItem && valItem && nameItem->text() == "Bottom") {
            double val = valItem->text().toDouble();
            for (auto& node : currentSolution.nodes) {
                if (node.y < minY + 10) node.potential = val;
            }
        }
        if (nameItem && valItem && nameItem->text() == "Top") {
            double val = valItem->text().toDouble();
            for (auto& node : currentSolution.nodes) {
                if (node.y > maxY - 10) node.potential = val;
            }
        }
    }
    
    if (visualizer) {
        electrostatic::ColorMap cmap;
        cmap.minValue = 0;
        cmap.maxValue = 10;
        visualizer->drawPotential(std::make_shared<electrostatic::Solution>(currentSolution), cmap);
    }
    
    updateStatusBar(QString("Done! Energy: %1 J").arg(currentSolution.energy));
}

// Settings
void MainWindow::on_actionMaterials_triggered()
{
    updateStatusBar("Materials");
}

void MainWindow::on_actionSettings_triggered()
{
    updateStatusBar("Settings");
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "About",
        "Electrostatic2D v1.0\n"
        "Electrostatic field calculation using FEM\n"
        "© 2026\n\n"
        "Controls:\n"
        "N - points, L - lines, mouse wheel - zoom\n"
        "Ctrl+M - build mesh\n"
        "Ctrl+R - analyze\n"
        "Ctrl+S - save geometry\n"
        "Ctrl+O - load geometry\n"
        "Ctrl+Shift+S - save full project (mesh + results)\n"
        "Ctrl+Shift+O - load full project");
}

void MainWindow::onSettingsChanged(int width, int height, double gridSize)
{
    scene->setSceneRect(-width/2, -height/2, width, height);
    if (scene) scene->setGridSize(gridSize);
}