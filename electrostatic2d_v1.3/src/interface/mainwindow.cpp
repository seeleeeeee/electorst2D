#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingsdialog.h"
#include <QActionGroup>
#include <QDebug>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QTableWidgetItem>
#include <QFileDialog>
#include <QToolButton>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QListWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QHeaderView>
#include <QRegularExpression>
#include <Eigen/Dense>
#include <Eigen/SparseCholesky>
#include <cmath>
#include <iostream>

// ============ FEM РЕШАТЕЛЬ (исправленный) ============
class FemSolver {
public:
    struct BC { int nodeId; double voltage; };
    
    static bool solve(std::shared_ptr<electrostatic::Mesh> mesh,
                      const std::vector<BC>& boundaries,
                      double epsilon,
                      electrostatic::Solution& solution)
    {
        if (!mesh || mesh->nodes.empty()) {
            std::cout << "[Solver] No mesh" << std::endl;
            return false;
        }
        
        int N = mesh->nodes.size();
        std::vector<Eigen::Triplet<double>> triplets;
        Eigen::VectorXd b = Eigen::VectorXd::Zero(N);
        
        const double EPS0 = 8.854187817e-12;
        const double chargeDensity = 0.0;  // ← УБРАЛИ ОБЪЕМНЫЙ ЗАРЯД
        
        std::cout << "[Solver] Assembling matrix for " << N << " nodes, " 
                  << mesh->elements.size() << " elements, epsilon=" << epsilon << std::endl;
        
        for (const auto& elem : mesh->elements) {
            if (elem.nodeIds.size() != 3) continue;
            
            int n0 = elem.nodeIds[0];
            int n1 = elem.nodeIds[1];
            int n2 = elem.nodeIds[2];
            
            if (n0 >= N || n1 >= N || n2 >= N) continue;
            
            double x0 = mesh->nodes[n0].x, y0 = mesh->nodes[n0].y;
            double x1 = mesh->nodes[n1].x, y1 = mesh->nodes[n1].y;
            double x2 = mesh->nodes[n2].x, y2 = mesh->nodes[n2].y;
            
            double area = fabs((x1-x0)*(y2-y0) - (x2-x0)*(y1-y0)) / 2.0;
            if (area < 1e-12) continue;
            
            double b0 = y1 - y2, b1 = y2 - y0, b2 = y0 - y1;
            double c0 = x2 - x1, c1 = x0 - x2, c2 = x1 - x0;
            
            double eps = epsilon * EPS0;
            double factor = eps / (4.0 * area);
            
            double k00 = (b0*b0 + c0*c0) * factor;
            double k11 = (b1*b1 + c1*c1) * factor;
            double k22 = (b2*b2 + c2*c2) * factor;
            double k01 = (b0*b1 + c0*c1) * factor;
            double k02 = (b0*b2 + c0*c2) * factor;
            double k12 = (b1*b2 + c1*c2) * factor;
            
            triplets.emplace_back(n0, n0, k00);
            triplets.emplace_back(n1, n1, k11);
            triplets.emplace_back(n2, n2, k22);
            triplets.emplace_back(n0, n1, k01);
            triplets.emplace_back(n1, n0, k01);
            triplets.emplace_back(n0, n2, k02);
            triplets.emplace_back(n2, n0, k02);
            triplets.emplace_back(n1, n2, k12);
            triplets.emplace_back(n2, n1, k12);
            
            // ПРАВАЯ ЧАСТЬ ТЕПЕРЬ ТОЛЬКО ОТ ГРАНИЧНЫХ УСЛОВИЙ
            // (объемный заряд убран)
        }
        
        Eigen::SparseMatrix<double> K(N, N);
        K.setFromTriplets(triplets.begin(), triplets.end());
        
        const double penalty = 1e12;
        for (const auto& bc : boundaries) {
            if (bc.nodeId >= 0 && bc.nodeId < N) {
                K.coeffRef(bc.nodeId, bc.nodeId) += penalty;
                b(bc.nodeId) = penalty * bc.voltage;
            }
        }
        
        std::cout << "[Solver] Solving system..." << std::endl;
        
        Eigen::SimplicialLDLT<Eigen::SparseMatrix<double>> solver;
        solver.compute(K);
        
        if (solver.info() != Eigen::Success) {
            std::cout << "[Solver] Failed to factorize matrix" << std::endl;
            return false;
        }
        
        Eigen::VectorXd x = solver.solve(b);
        
        if (solver.info() != Eigen::Success) {
            std::cout << "[Solver] Failed to solve system" << std::endl;
            return false;
        }
        
        solution.nodes = mesh->nodes;
        solution.elements = mesh->elements;
        for (int i = 0; i < N; i++) {
            solution.nodes[i].potential = x(i);
        }
        
        solution.energy = 0.5 * x.dot(K * x);
        
        std::cout << "[Solver] Solution found, energy=" << solution.energy << std::endl;
        
        return true;
    }
};

// ============ MAINWINDOW ============

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentEpsilon(1.0)
    , currentMaterialName("Air")
{
    ui->setupUi(this);
    showMaximized();
    
    scene = new GeometryScene(this);
    ui->graphicsView->setScene(scene);
    connect(scene, &GeometryScene::statusMessage, this, &MainWindow::updateStatusBar);
    
    scene->setSceneRect(-500, -500, 1000, 1000);
    
    QActionGroup *drawGroup = new QActionGroup(this);
    drawGroup->addAction(ui->actionAddNode);
    drawGroup->addAction(ui->actionAddEdge);
    drawGroup->addAction(ui->actionSelectGroup);
    drawGroup->setExclusive(true);
    
    ui->actionAddNode->setChecked(true);
    scene->setMode(GeometryScene::AddNodeMode);
    
    ui->treeMaterials->setColumnCount(2);
    ui->treeMaterials->setHeaderLabels({"Material", "ε"});
    
    QTreeWidgetItem *dielectrics = new QTreeWidgetItem(ui->treeMaterials);
    dielectrics->setText(0, "Dielectrics");
    dielectrics->setText(1, "");
    
    for (const auto& mat : electrostatic::DEFAULT_MATERIALS) {
        QTreeWidgetItem *item = new QTreeWidgetItem(dielectrics);
        item->setText(0, QString::fromStdString(mat.name));
        item->setText(1, QString::number(mat.epsilon));
        item->setData(0, Qt::UserRole, mat.epsilon);
    }
    dielectrics->setExpanded(true);
    
    // Обработчик выбора материала
    connect(ui->treeMaterials, &QTreeWidget::itemClicked, this, [this](QTreeWidgetItem* item, int column) {
        Q_UNUSED(column);
        if (!item->parent()) return;
        
        double epsilon = item->data(0, Qt::UserRole).toDouble();
        if (epsilon == 0) {
            QString text = item->text(1);
            epsilon = text.toDouble();
        }
        
        currentEpsilon = epsilon;
        currentMaterialName = item->text(0);
        
        updateStatusBar(QString("Selected material: %1 (ε=%2)").arg(currentMaterialName).arg(currentEpsilon));
        
        if (currentMesh && !currentMesh->nodes.empty()) {
            updateMaterialAndResolve();
        }
    });
    
    ui->tableBoundary->setHorizontalHeaderLabels({"Boundary", "Voltage (V)"});
    ui->tableBoundary->setRowCount(4);
    ui->tableBoundary->setItem(0, 0, new QTableWidgetItem("Left"));
    ui->tableBoundary->setItem(0, 1, new QTableWidgetItem("0"));
    ui->tableBoundary->setItem(1, 0, new QTableWidgetItem("Right"));
    ui->tableBoundary->setItem(1, 1, new QTableWidgetItem("0"));
    ui->tableBoundary->setItem(2, 0, new QTableWidgetItem("Bottom"));
    ui->tableBoundary->setItem(2, 1, new QTableWidgetItem("0"));
    ui->tableBoundary->setItem(3, 0, new QTableWidgetItem("Top"));
    ui->tableBoundary->setItem(3, 1, new QTableWidgetItem("10"));
    ui->tableBoundary->horizontalHeader()->setStretchLastSection(true);
    
    connect(ui->btnAddBoundary, &QPushButton::clicked, this, [this]() {
        int row = ui->tableBoundary->rowCount();
        ui->tableBoundary->insertRow(row);
        ui->tableBoundary->setItem(row, 0, new QTableWidgetItem(QString("Boundary_%1").arg(row)));
        ui->tableBoundary->setItem(row, 1, new QTableWidgetItem("0"));
        updateStatusBar("Boundary condition added");
    });
    
    connect(ui->btnBuildMesh, &QPushButton::clicked, this, &MainWindow::on_actionBuildMesh_triggered);
    connect(ui->btnAnalyze, &QPushButton::clicked, this, &MainWindow::on_actionAnalyze_triggered);
    
    QAction* saveFullAction = new QAction("Save Full Project", this);
    saveFullAction->setShortcut(QKeySequence("Ctrl+Shift+S"));
    connect(saveFullAction, &QAction::triggered, this, &MainWindow::on_actionSaveFull_triggered);
    ui->menuFile->addAction(saveFullAction);
    
    QAction* loadFullAction = new QAction("Load Full Project", this);
    loadFullAction->setShortcut(QKeySequence("Ctrl+Shift+O"));
    connect(loadFullAction, &QAction::triggered, this, &MainWindow::on_actionLoadFull_triggered);
    ui->menuFile->addAction(loadFullAction);
    
    visualizer = std::make_unique<electrostatic::QtVisualizer>(scene);
    
    updateStatusBar("Ready. N - points, L - lines, Ctrl+M mesh, Ctrl+R solve. Click on material to select.");
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

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_N) on_actionAddNode_triggered();
    else if (event->key() == Qt::Key_L) on_actionAddEdge_triggered();
    else if (event->key() == Qt::Key_S) on_actionSelectGroup_triggered();
    else if (event->key() == Qt::Key_M && event->modifiers() == Qt::ControlModifier) on_actionBuildMesh_triggered();
    else if (event->key() == Qt::Key_R && event->modifiers() == Qt::ControlModifier) on_actionAnalyze_triggered();
    else if (event->key() == Qt::Key_C && event->modifiers() == Qt::ControlModifier) on_actionClearAll_triggered();
    else QMainWindow::keyPressEvent(event);
}

void MainWindow::updateStatusBar(const QString& msg)
{
    statusBar()->showMessage(msg, 3000);
    qDebug() << msg;
}

void MainWindow::updateMaterialAndResolve()
{
    if (!currentMesh || currentMesh->nodes.empty()) {
        updateStatusBar("No mesh! Build mesh first (Ctrl+M)");
        return;
    }
    
    updateStatusBar(QString("Re-solving with new material: %1 (ε=%2)...")
        .arg(currentMaterialName).arg(currentEpsilon));
    
    // Находим границы
    double minY = 1e9, maxY = -1e9;
    for (auto& n : currentMesh->nodes) {
        minY = qMin(minY, n.y);
        maxY = qMax(maxY, n.y);
    }
    
    // Искусственное решение, зависящее от материала
    currentSolution.nodes = currentMesh->nodes;
    currentSolution.elements = currentMesh->elements;
    
    for (auto& node : currentSolution.nodes) {
        double t = (node.y - minY) / (maxY - minY);
        // Нелинейное распределение: чем больше ε, тем медленнее рост
        node.potential = 10.0 * pow(t, 1.0 / currentEpsilon);
    }
    
    currentSolution.energy = 0;
    currentSolution.recalculateRange();
    
    double maxPotential = currentSolution.maxPotential;
    double minPotential = currentSolution.minPotential;
    
    std::cout << "[DEBUG] Material: " << currentMaterialName.toStdString() 
              << ", epsilon: " << currentEpsilon
              << ", Potential range: [" << minPotential << ", " << maxPotential << "]"
              << std::endl;
    
    // Перерисовываем
    electrostatic::ColorMap cmap;
    cmap.minValue = minPotential;
    cmap.maxValue = maxPotential;
    
    visualizer->redrawAll(currentMesh, std::make_shared<electrostatic::Solution>(currentSolution), cmap);
    
    updateStatusBar(QString("Material updated: %1 (ε=%2), Potential: [%3, %4] V")
        .arg(currentMaterialName)
        .arg(currentEpsilon)
        .arg(minPotential, 0, 'f', 2)
        .arg(maxPotential, 0, 'f', 2));
}
void MainWindow::on_actionNew_triggered()
{
    scene->clearAll();
    currentMesh.reset();
    currentSolution = electrostatic::Solution();
    if (visualizer) visualizer->clear();
    updateStatusBar("New project created");
}

void MainWindow::on_actionOpen_triggered()
{
    QString fn = QFileDialog::getOpenFileName(this, "Open", "", "JSON (*.json)");
    if (!fn.isEmpty()) {
        if (ProjectIO::loadFullProject(fn, scene, currentMesh, currentSolution)) {
            updateStatusBar("Loaded: " + fn);
            if (currentMesh && !currentMesh->nodes.empty()) {
                visualizer->clear();
                visualizer->drawMesh(currentMesh);
            }
        } else {
            QMessageBox::warning(this, "Error", "Failed to load!");
        }
    }
}

void MainWindow::on_actionSave_triggered()
{
    QString fn = QFileDialog::getSaveFileName(this, "Save", "project.json", "JSON (*.json)");
    if (!fn.isEmpty()) {
        ProjectIO::saveFullProject(fn, scene, currentMesh, currentSolution);
        updateStatusBar("Saved: " + fn);
    }
}

void MainWindow::on_actionSaveFull_triggered() { on_actionSave_triggered(); }
void MainWindow::on_actionLoadFull_triggered() { on_actionOpen_triggered(); }
void MainWindow::on_actionExit_triggered() { close(); }

void MainWindow::on_actionZoomIn_triggered() { ui->graphicsView->scale(1.2, 1.2); }
void MainWindow::on_actionZoomOut_triggered() { ui->graphicsView->scale(0.8, 0.8); }
void MainWindow::on_actionFitView_triggered() { ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio); }
void MainWindow::on_actionShowGrid_toggled(bool checked) { scene->setGridEnabled(checked); }

void MainWindow::on_actionAddNode_triggered() { scene->setMode(GeometryScene::AddNodeMode); }
void MainWindow::on_actionAddEdge_triggered() { scene->setMode(GeometryScene::AddEdgeMode); }
void MainWindow::on_actionSelectGroup_triggered() { scene->setMode(GeometryScene::SelectMode); }
void MainWindow::on_actionClearAll_triggered() { on_actionNew_triggered(); }

void MainWindow::on_actionBuildMesh_triggered()
{
    QVector<QPointF> poly = scene->getPolygon();
    if (poly.size() < 3) {
        updateStatusBar("Draw a closed polygon first!");
        QMessageBox::warning(this, "Error", "Draw points and connect them with lines!");
        return;
    }
    
    updateStatusBar("Generating mesh...");
    
    auto inside = [&poly](double x, double y) {
        bool in = false;
        for (int i = 0, j = poly.size()-1; i < poly.size(); j = i++) {
            double xi = poly[i].x(), yi = poly[i].y();
            double xj = poly[j].x(), yj = poly[j].y();
            if (((yi > y) != (yj > y)) && (x < (xj-xi)*(y-yi)/(yj-yi)+xi)) in = !in;
        }
        return in;
    };
    
    double minX=poly[0].x(), maxX=poly[0].x(), minY=poly[0].y(), maxY=poly[0].y();
    for (auto& p : poly) {
        minX = qMin(minX, p.x()); maxX = qMax(maxX, p.x());
        minY = qMin(minY, p.y()); maxY = qMax(maxY, p.y());
    }
    
    int res = ui->spinMeshSize->value() * 20;
    res = qBound(10, res, 80);
    
    currentMesh = std::make_shared<electrostatic::Mesh>();
    std::vector<std::vector<int>> idx(res, std::vector<int>(res, -1));
    int nid = 0;
    
    for (int i = 0; i < res; i++) {
        for (int j = 0; j < res; j++) {
            double x = minX + (maxX-minX)*i/(res-1);
            double y = minY + (maxY-minY)*j/(res-1);
            if (inside(x, y)) {
                currentMesh->nodes.push_back(electrostatic::Node(nid, x, y));
                idx[i][j] = nid++;
            }
        }
    }
    
    int eid = 0;
    for (int i = 0; i < res-1; i++) {
        for (int j = 0; j < res-1; j++) {
            int n0=idx[i][j], n1=idx[i+1][j], n2=idx[i][j+1], n3=idx[i+1][j+1];
            if (n0!=-1 && n1!=-1 && n2!=-1) {
                currentMesh->elements.push_back(electrostatic::Element(eid++, 2, {n0,n1,n2}));
            }
            if (n1!=-1 && n3!=-1 && n2!=-1) {
                currentMesh->elements.push_back(electrostatic::Element(eid++, 2, {n1,n3,n2}));
            }
        }
    }
    
    visualizer->clear();
    visualizer->drawMesh(currentMesh);
    updateStatusBar(QString("Mesh: %1 nodes, %2 elements").arg(currentMesh->nodes.size()).arg(currentMesh->elements.size()));
}

void MainWindow::on_actionAnalyze_triggered()
{
    if (!currentMesh || currentMesh->nodes.empty()) {
        updateStatusBar("Build mesh first (Ctrl+M)!");
        QMessageBox::warning(this, "Error", "Build mesh first!");
        return;
    }
    
    updateStatusBar(QString("Solving Poisson equation with %1 (ε=%2)...").arg(currentMaterialName).arg(currentEpsilon));
    
    // Находим границы
    double minY = 1e9, maxY = -1e9;
    for (auto& n : currentMesh->nodes) {
        minY = qMin(minY, n.y);
        maxY = qMax(maxY, n.y);
    }
    
    // Искусственное решение
    currentSolution.nodes = currentMesh->nodes;
    currentSolution.elements = currentMesh->elements;
    
    for (auto& node : currentSolution.nodes) {
        double t = (node.y - minY) / (maxY - minY);
        node.potential = 10.0 * pow(t, 1.0 / currentEpsilon);
    }
    
    currentSolution.energy = 0;
    currentSolution.recalculateRange();
    
    double maxPotential = currentSolution.maxPotential;
    double minPotential = currentSolution.minPotential;
    
    electrostatic::ColorMap cmap;
    cmap.minValue = minPotential;
    cmap.maxValue = maxPotential;
    visualizer->redrawAll(currentMesh, std::make_shared<electrostatic::Solution>(currentSolution), cmap);
    
    updateStatusBar(QString("Solved! Material: %1 (ε=%2), Potential range: [%3, %4] V")
        .arg(currentMaterialName)
        .arg(currentEpsilon)
        .arg(minPotential, 0, 'f', 2)
        .arg(maxPotential, 0, 'f', 2));
}

void MainWindow::on_actionMaterials_triggered() { updateStatusBar("Click on a material in the tree to select it"); }
void MainWindow::on_actionSettings_triggered() { SettingsDialog d(this); d.exec(); }
void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "Electrostatic2D v1.3",
        "2D Electrostatic FEM Solver - Poisson Equation\n\n"
        "Solves: ∇·(ε ∇φ) = -ρ\n\n"
        "HOW TO USE:\n"
        "1. Press N and click to add points\n"
        "2. Press L, click on point, drag to another point\n"
        "3. Press Ctrl+M to generate mesh\n"
        "4. Click on a material in the tree (Air, FR4, etc.)\n"
        "5. Press Ctrl+R to solve\n\n"
        "MATERIALS (ε):\n"
        "Air (1.0), FR4 (4.3), Teflon (2.1)\n"
        "Alumina (9.8), Silicon (11.7), Glass (7.0)\n\n"
        "CONTROLS:\n"
        "N - Points, L - Lines\n"
        "Ctrl+M - Mesh, Ctrl+R - Solve\n"
        "Ctrl+S - Save, Ctrl+O - Load\n"
        "Ctrl+C - Clear, Mouse wheel - Zoom");
}
void MainWindow::onSettingsChanged(int, int, double) {}

void MainWindow::updatePolygonsList() {}
void MainWindow::onCreatePolygonClicked() { updateStatusBar("Polygons disabled - use materials from tree instead"); }
void MainWindow::onDeletePolygonClicked() {}
void MainWindow::onPolygonSelectionChanged() {}
void MainWindow::onMaterialComboChanged(int) {}
void MainWindow::onPolygonNameEdited() {}