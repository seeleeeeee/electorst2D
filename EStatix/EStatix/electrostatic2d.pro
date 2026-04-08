QT += core gui widgets

TARGET = electrostatic2d
TEMPLATE = app

CONFIG += c++17
CONFIG += static
CONFIG += console

# Статическая линковка
QMAKE_LFLAGS += -static -static-libgcc -static-libstdc++
QMAKE_LFLAGS += -Wl,-Bstatic

SOURCES += \
    main.cpp \
    src/interface/launcherwindow.cpp \
    src/interface/mainwindow.cpp \
    src/interface/geometry_scene.cpp \
    src/interface/materialsdialog.cpp \
    src/interface/settingsdialog.cpp

HEADERS += \
    src/interface/launcherwindow.h \
    src/interface/mainwindow.h \
    src/interface/geometry_scene.h \
    src/interface/materialsdialog.h \
    src/interface/settingsdialog.h \
    src/interface/qt_visualizer.h \
    src/interface/project_io.h \
    src/interfaces/ipreprocessor.h \
    src/interfaces/isolver.h \
    src/interfaces/ivisualizer.h \
    src/structures/electrostatic_structures.h \
    src/core/preprocessor/preprocessor_core.h \
    src/designer/solver_adapter.h

FORMS += \
    src/interface/ui/launcherwindow.ui \
    src/interface/ui/mainwindow.ui \
    src/interface/ui/materialsdialog.ui \
    src/interface/ui/settingsdialog.ui