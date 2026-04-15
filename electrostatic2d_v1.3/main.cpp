#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "interface/launcherwindow.h"

void loadStyleSheet(QApplication& app) {
    QFile file(":/style.css");
    if (!file.exists()) {
        file.setFileName("style.css");
    }
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        app.setStyleSheet(styleSheet);
        qDebug() << "Style loaded successfully";
    } else {
        qDebug() << "Failed to load style.css";
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    loadStyleSheet(app);
    
    LauncherWindow launcher;
    launcher.show();
    
    return app.exec();
}