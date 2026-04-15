/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionExit;
    QAction *actionZoomIn;
    QAction *actionZoomOut;
    QAction *actionFitView;
    QAction *actionShowGrid;
    QAction *actionAddNode;
    QAction *actionAddEdge;
    QAction *actionSelectGroup;
    QAction *actionClearAll;
    QAction *actionAbout;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBoxMaterials;
    QVBoxLayout *verticalLayout_2;
    QTreeWidget *treeMaterials;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btnAddMaterial;
    QPushButton *btnRemoveMaterial;
    QGroupBox *groupBoxBoundary;
    QVBoxLayout *verticalLayout_3;
    QTableWidget *tableBoundary;
    QPushButton *btnAddBoundary;
    QGroupBox *groupBoxMesh;
    QFormLayout *formLayout;
    QLabel *label;
    QDoubleSpinBox *spinMeshSize;
    QLabel *label_2;
    QCheckBox *chkAdaptive;
    QSpacerItem *verticalSpacer;
    QPushButton *btnBuildMesh;
    QPushButton *btnAnalyze;
    QGraphicsView *graphicsView;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuView;
    QMenu *menuTools;
    QMenu *menuHelp;
    QToolBar *toolBar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1200, 800);
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName(QString::fromUtf8("actionNew"));
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionZoomIn = new QAction(MainWindow);
        actionZoomIn->setObjectName(QString::fromUtf8("actionZoomIn"));
        actionZoomOut = new QAction(MainWindow);
        actionZoomOut->setObjectName(QString::fromUtf8("actionZoomOut"));
        actionFitView = new QAction(MainWindow);
        actionFitView->setObjectName(QString::fromUtf8("actionFitView"));
        actionShowGrid = new QAction(MainWindow);
        actionShowGrid->setObjectName(QString::fromUtf8("actionShowGrid"));
        actionShowGrid->setCheckable(true);
        actionShowGrid->setChecked(true);
        actionAddNode = new QAction(MainWindow);
        actionAddNode->setObjectName(QString::fromUtf8("actionAddNode"));
        actionAddNode->setCheckable(true);
        actionAddEdge = new QAction(MainWindow);
        actionAddEdge->setObjectName(QString::fromUtf8("actionAddEdge"));
        actionAddEdge->setCheckable(true);
        actionSelectGroup = new QAction(MainWindow);
        actionSelectGroup->setObjectName(QString::fromUtf8("actionSelectGroup"));
        actionSelectGroup->setCheckable(true);
        actionClearAll = new QAction(MainWindow);
        actionClearAll->setObjectName(QString::fromUtf8("actionClearAll"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setSpacing(5);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(5, 5, 5, 5);
        dockWidget = new QDockWidget(centralwidget);
        dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
        dockWidget->setMinimumSize(QSize(300, 0));
        dockWidget->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetMovable);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        verticalLayout = new QVBoxLayout(dockWidgetContents);
        verticalLayout->setSpacing(10);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBoxMaterials = new QGroupBox(dockWidgetContents);
        groupBoxMaterials->setObjectName(QString::fromUtf8("groupBoxMaterials"));
        verticalLayout_2 = new QVBoxLayout(groupBoxMaterials);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        treeMaterials = new QTreeWidget(groupBoxMaterials);
        treeMaterials->setObjectName(QString::fromUtf8("treeMaterials"));
        treeMaterials->setHeaderHidden(true);

        verticalLayout_2->addWidget(treeMaterials);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        btnAddMaterial = new QPushButton(groupBoxMaterials);
        btnAddMaterial->setObjectName(QString::fromUtf8("btnAddMaterial"));

        horizontalLayout_2->addWidget(btnAddMaterial);

        btnRemoveMaterial = new QPushButton(groupBoxMaterials);
        btnRemoveMaterial->setObjectName(QString::fromUtf8("btnRemoveMaterial"));

        horizontalLayout_2->addWidget(btnRemoveMaterial);


        verticalLayout_2->addLayout(horizontalLayout_2);


        verticalLayout->addWidget(groupBoxMaterials);

        groupBoxBoundary = new QGroupBox(dockWidgetContents);
        groupBoxBoundary->setObjectName(QString::fromUtf8("groupBoxBoundary"));
        verticalLayout_3 = new QVBoxLayout(groupBoxBoundary);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        tableBoundary = new QTableWidget(groupBoxBoundary);
        if (tableBoundary->columnCount() < 2)
            tableBoundary->setColumnCount(2);
        if (tableBoundary->rowCount() < 2)
            tableBoundary->setRowCount(2);
        tableBoundary->setObjectName(QString::fromUtf8("tableBoundary"));
        tableBoundary->setColumnCount(2);
        tableBoundary->horizontalHeader()->setVisible(true);
        tableBoundary->verticalHeader()->setVisible(false);

        verticalLayout_3->addWidget(tableBoundary);

        btnAddBoundary = new QPushButton(groupBoxBoundary);
        btnAddBoundary->setObjectName(QString::fromUtf8("btnAddBoundary"));

        verticalLayout_3->addWidget(btnAddBoundary);


        verticalLayout->addWidget(groupBoxBoundary);

        groupBoxMesh = new QGroupBox(dockWidgetContents);
        groupBoxMesh->setObjectName(QString::fromUtf8("groupBoxMesh"));
        formLayout = new QFormLayout(groupBoxMesh);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label = new QLabel(groupBoxMesh);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        spinMeshSize = new QDoubleSpinBox(groupBoxMesh);
        spinMeshSize->setObjectName(QString::fromUtf8("spinMeshSize"));
        spinMeshSize->setMinimum(0.100000000000000);
        spinMeshSize->setMaximum(100.000000000000000);
        spinMeshSize->setValue(1.000000000000000);

        formLayout->setWidget(0, QFormLayout::FieldRole, spinMeshSize);

        label_2 = new QLabel(groupBoxMesh);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        chkAdaptive = new QCheckBox(groupBoxMesh);
        chkAdaptive->setObjectName(QString::fromUtf8("chkAdaptive"));

        formLayout->setWidget(1, QFormLayout::FieldRole, chkAdaptive);


        verticalLayout->addWidget(groupBoxMesh);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        btnBuildMesh = new QPushButton(dockWidgetContents);
        btnBuildMesh->setObjectName(QString::fromUtf8("btnBuildMesh"));

        verticalLayout->addWidget(btnBuildMesh);

        btnAnalyze = new QPushButton(dockWidgetContents);
        btnAnalyze->setObjectName(QString::fromUtf8("btnAnalyze"));

        verticalLayout->addWidget(btnAnalyze);

        dockWidget->setWidget(dockWidgetContents);

        horizontalLayout->addWidget(dockWidget);

        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(graphicsView->sizePolicy().hasHeightForWidth());
        graphicsView->setSizePolicy(sizePolicy);
        graphicsView->setMinimumSize(QSize(500, 400));
        graphicsView->setRenderHints(QPainter::Antialiasing);
        graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
        graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        graphicsView->setResizeAnchor(QGraphicsView::AnchorUnderMouse);

        horizontalLayout->addWidget(graphicsView);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1200, 22));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuView = new QMenu(menubar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menuTools = new QMenu(menubar);
        menuTools->setObjectName(QString::fromUtf8("menuTools"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow->setMenuBar(menubar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuView->menuAction());
        menubar->addAction(menuTools->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuView->addAction(actionZoomIn);
        menuView->addAction(actionZoomOut);
        menuView->addAction(actionFitView);
        menuView->addSeparator();
        menuView->addAction(actionShowGrid);
        menuTools->addAction(actionAddNode);
        menuTools->addAction(actionAddEdge);
        menuTools->addAction(actionSelectGroup);
        menuTools->addSeparator();
        menuTools->addAction(actionClearAll);
        menuHelp->addAction(actionAbout);
        toolBar->addAction(actionAddNode);
        toolBar->addAction(actionAddEdge);
        toolBar->addAction(actionSelectGroup);
        toolBar->addSeparator();
        toolBar->addAction(actionZoomIn);
        toolBar->addAction(actionZoomOut);
        toolBar->addSeparator();
        toolBar->addAction(actionClearAll);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Electrostatic2D - \320\240\320\260\321\201\321\207\320\265\321\202 \321\215\320\273\320\265\320\272\321\202\321\200\320\276\321\201\321\202\320\260\321\202\320\270\321\207\320\265\321\201\320\272\320\270\321\205 \320\277\320\276\320\273\320\265\320\271", nullptr));
        actionNew->setText(QCoreApplication::translate("MainWindow", "\320\235\320\276\320\262\321\213\320\271", nullptr));
#if QT_CONFIG(shortcut)
        actionNew->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+N", nullptr));
#endif // QT_CONFIG(shortcut)
        actionOpen->setText(QCoreApplication::translate("MainWindow", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214", nullptr));
#if QT_CONFIG(shortcut)
        actionOpen->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSave->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", nullptr));
#if QT_CONFIG(shortcut)
        actionSave->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionExit->setText(QCoreApplication::translate("MainWindow", "\320\222\321\213\321\205\320\276\320\264", nullptr));
#if QT_CONFIG(shortcut)
        actionExit->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_CONFIG(shortcut)
        actionZoomIn->setText(QCoreApplication::translate("MainWindow", "\320\237\321\200\320\270\320\261\320\273\320\270\320\267\320\270\321\202\321\214", nullptr));
#if QT_CONFIG(shortcut)
        actionZoomIn->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl++", nullptr));
#endif // QT_CONFIG(shortcut)
        actionZoomOut->setText(QCoreApplication::translate("MainWindow", "\320\236\321\202\320\264\320\260\320\273\320\270\321\202\321\214", nullptr));
#if QT_CONFIG(shortcut)
        actionZoomOut->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+-", nullptr));
#endif // QT_CONFIG(shortcut)
        actionFitView->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\272\320\260\320\267\320\260\321\202\321\214 \320\262\321\201\321\221", nullptr));
        actionShowGrid->setText(QCoreApplication::translate("MainWindow", "\320\241\320\265\321\202\320\272\320\260", nullptr));
        actionAddNode->setText(QCoreApplication::translate("MainWindow", "\320\242\320\276\321\207\320\272\320\260", nullptr));
#if QT_CONFIG(shortcut)
        actionAddNode->setShortcut(QCoreApplication::translate("MainWindow", "N", nullptr));
#endif // QT_CONFIG(shortcut)
        actionAddEdge->setText(QCoreApplication::translate("MainWindow", "\320\233\320\270\320\275\320\270\321\217", nullptr));
#if QT_CONFIG(shortcut)
        actionAddEdge->setShortcut(QCoreApplication::translate("MainWindow", "L", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSelectGroup->setText(QCoreApplication::translate("MainWindow", "\320\222\321\213\320\264\320\265\320\273\320\265\320\275\320\270\320\265", nullptr));
#if QT_CONFIG(shortcut)
        actionSelectGroup->setShortcut(QCoreApplication::translate("MainWindow", "S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionClearAll->setText(QCoreApplication::translate("MainWindow", "\320\236\321\207\320\270\321\201\321\202\320\270\321\202\321\214", nullptr));
#if QT_CONFIG(shortcut)
        actionClearAll->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+C", nullptr));
#endif // QT_CONFIG(shortcut)
        actionAbout->setText(QCoreApplication::translate("MainWindow", "\320\236 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\265", nullptr));
        dockWidget->setWindowTitle(QCoreApplication::translate("MainWindow", "\320\237\320\260\321\200\320\260\320\274\320\265\321\202\321\200\321\213", nullptr));
        groupBoxMaterials->setTitle(QCoreApplication::translate("MainWindow", "\320\234\320\260\321\202\320\265\321\200\320\270\320\260\320\273\321\213", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = treeMaterials->headerItem();
        ___qtreewidgetitem->setText(0, QCoreApplication::translate("MainWindow", "\320\234\320\260\321\202\320\265\321\200\320\270\320\260\320\273", nullptr));
        btnAddMaterial->setText(QCoreApplication::translate("MainWindow", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214", nullptr));
        btnRemoveMaterial->setText(QCoreApplication::translate("MainWindow", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214", nullptr));
        groupBoxBoundary->setTitle(QCoreApplication::translate("MainWindow", "\320\223\321\200\320\260\320\275\320\270\321\207\320\275\321\213\320\265 \321\203\321\201\320\273\320\276\320\262\320\270\321\217", nullptr));
        btnAddBoundary->setText(QCoreApplication::translate("MainWindow", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \320\263\321\200\320\260\320\275\320\270\321\206\321\203", nullptr));
        groupBoxMesh->setTitle(QCoreApplication::translate("MainWindow", "\320\237\320\260\321\200\320\260\320\274\320\265\321\202\321\200\321\213 \321\201\320\265\321\202\320\272\320\270", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\320\240\320\260\320\267\320\274\320\265\321\200 \321\215\320\273\320\265\320\274\320\265\320\275\321\202\320\260:", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\320\220\320\264\320\260\320\277\321\202\320\270\320\262\320\275\320\260\321\217:", nullptr));
        chkAdaptive->setText(QString());
        btnBuildMesh->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\321\201\321\202\321\200\320\276\320\270\321\202\321\214 \321\201\320\265\321\202\320\272\321\203", nullptr));
#if QT_CONFIG(shortcut)
        btnBuildMesh->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+M", nullptr));
#endif // QT_CONFIG(shortcut)
        btnAnalyze->setText(QCoreApplication::translate("MainWindow", "\320\220\320\275\320\260\320\273\320\270\320\267", nullptr));
#if QT_CONFIG(shortcut)
        btnAnalyze->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+R", nullptr));
#endif // QT_CONFIG(shortcut)
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "\320\244\320\260\320\271\320\273", nullptr));
        menuView->setTitle(QCoreApplication::translate("MainWindow", "\320\222\320\270\320\264", nullptr));
        menuTools->setTitle(QCoreApplication::translate("MainWindow", "\320\230\320\275\321\201\321\202\321\200\321\203\320\274\320\265\320\275\321\202\321\213", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\274\320\276\321\211\321\214", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "\320\230\320\275\321\201\321\202\321\200\321\203\320\274\320\265\320\275\321\202\321\213", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
