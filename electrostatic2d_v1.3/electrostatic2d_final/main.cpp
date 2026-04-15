#include <QApplication>
#include "src/interface/launcherwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    LauncherWindow launcher;
    launcher.show();
    return app.exec();
}