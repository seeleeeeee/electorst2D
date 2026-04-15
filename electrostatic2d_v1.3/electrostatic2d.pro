QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 static
CONFIG += console

TARGET = Electrostatic2D
TEMPLATE = app

# Eigen path
EIGEN_PATH = D:/projectsdd/eigen-5.0.0
INCLUDEPATH += $$EIGEN_PATH

win32: {
    QMAKE_LFLAGS += -static -static-libgcc -static-libstdc++
    QMAKE_CXXFLAGS += -static-libgcc -static-libstdc++
}

INCLUDEPATH += src \
               src/interface \
               src/structures

SOURCES += \
    main.cpp \
    src/interface/geometry_scene.cpp \
    src/interface/launcherwindow.cpp \
    src/interface/mainwindow.cpp \
    src/interface/materialsdialog.cpp \
    src/interface/settingsdialog.cpp

HEADERS += \
    src/interface/geometry_scene.h \
    src/interface/launcherwindow.h \
    src/interface/mainwindow.h \
    src/interface/materialsdialog.h \
    src/interface/settingsdialog.h \
    src/interface/project_io.h \
    src/interface/qt_visualizer.h \
    src/structures/electrostatic_structures.h

FORMS += \
    src/interface/ui/launcherwindow.ui \
    src/interface/ui/mainwindow.ui \
    src/interface/ui/materialsdialog.ui \
    src/interface/ui/settingsdialog.ui

win32: {
    CONFIG(debug, debug|release) {
        DESTDIR = $$PWD/debug
    } else {
        DESTDIR = $$PWD/release
    }
}