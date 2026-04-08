#include <QApplication>
#include <QFile>
#include <QDebug>
#include "src/interface/launcherwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Загружаем CSS стиль
    QFile styleFile("style.css");
    if (styleFile.open(QFile::ReadOnly)) {
        QString style = QLatin1String(styleFile.readAll());
        app.setStyleSheet(style);
        qDebug() << "Style loaded successfully";
    } else {
        qDebug() << "Failed to load style.css";
    }
    
    LauncherWindow launcher;
    launcher.show();
    return app.exec();
}