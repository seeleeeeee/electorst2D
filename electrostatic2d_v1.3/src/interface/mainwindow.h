#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWheelEvent>
#include <memory>
#include <vector>
#include <QListWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTreeWidgetItem>
#include "geometry_scene.h"
#include "qt_visualizer.h"
#include "project_io.h"
#include "../structures/electrostatic_structures.h"

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
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSaveFull_triggered();
    void on_actionLoadFull_triggered();
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
    
    // НОВЫЙ МЕТОД - обновление материала без перестроения сетки
    void updateMaterialAndResolve();
    
    // Полигоны (заглушки)
    void updatePolygonsList();
    void onCreatePolygonClicked();
    void onDeletePolygonClicked();
    void onPolygonSelectionChanged();
    void onMaterialComboChanged(int index);
    void onPolygonNameEdited();

private:
    Ui::MainWindow *ui;
    GeometryScene* scene;
    std::unique_ptr<electrostatic::QtVisualizer> visualizer;
    std::shared_ptr<electrostatic::Mesh> currentMesh;
    electrostatic::Solution currentSolution;
    std::vector<electrostatic::Polygon> currentPolygons;
    
    double currentEpsilon;
    QString currentMaterialName;
    
    QListWidget* polygonListWidget;
    QPushButton* btnCreatePolygon;
    QPushButton* btnDeletePolygon;
    QComboBox* materialCombo;
    QLineEdit* polygonNameEdit;
};

#endif