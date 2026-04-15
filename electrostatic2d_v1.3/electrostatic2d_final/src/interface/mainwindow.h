#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMessageBox>
#include <QMainWindow>
#include <QWheelEvent>
#include "geometry_scene.h"
#include "qt_visualizer.h"
#include <memory>
#include "../interfaces/ipreprocessor.h"
#include "../interfaces/isolver.h"
#include "../interfaces/ivisualizer.h"
#include "../structures/electrostatic_structures.h"
#include "../core/preprocessor/preprocessor_core.h"
#include "../designer/solver_adapter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void wheelEvent(QWheelEvent *event) override;

private slots:
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionExit_triggered();
    
    void on_actionZoomIn_triggered();
    void on_actionZoomOut_triggered();
    void on_actionFitView_triggered();
    
    void on_actionShowGrid_toggled(bool checked);
    
    void on_actionAddNode_triggered();
    void on_actionAddEdge_triggered();
    void on_actionSelectGroup_triggered();
    void on_actionClearAll_triggered();
    void on_actionBuildMesh_triggered();
    void on_actionAnalyze_triggered();
    
    void on_actionMaterials_triggered();
    void on_actionSettings_triggered();
    void on_actionAbout_triggered();
    
    void updateStatusBar(const QString& msg);
    void onSettingsChanged(int width, int height, double gridSize);

private:
    Ui::MainWindow *ui;
    GeometryScene* scene;
    
    std::unique_ptr<electrostatic::IPreprocessor> preprocessor;
    std::unique_ptr<electrostatic::ISolver> solver;
    std::unique_ptr<electrostatic::IVisualizer> visualizer;
    std::shared_ptr<electrostatic::Mesh> currentMesh;
    electrostatic::Solution currentSolution;
    electrostatic::Geometry currentGeometry;
    electrostatic::Materials currentMaterials;
};

#endif