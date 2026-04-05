#include "launcherwindow.h"
#include "ui_launcherwindow.h"
#include "mainwindow.h"

LauncherWindow::LauncherWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LauncherWindow)
{
    ui->setupUi(this);
}

LauncherWindow::~LauncherWindow()
{
    delete ui;
}

void LauncherWindow::on_btnNew_clicked()
{
    MainWindow *mainWin = new MainWindow();
    mainWin->show();
    this->close();
}

void LauncherWindow::on_btnOpen_clicked()
{
    MainWindow *mainWin = new MainWindow();
    mainWin->show();
    this->close();
}