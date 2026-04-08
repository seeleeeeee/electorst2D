#ifndef LAUNCHERWINDOW_H
#define LAUNCHERWINDOW_H

#include <QWidget>

namespace Ui {
class LauncherWindow;
}

class LauncherWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LauncherWindow(QWidget *parent = nullptr);
    ~LauncherWindow();

signals:
    void projectReady();

private slots:
    void on_btnNew_clicked();
    void on_btnOpen_clicked();

private:
    Ui::LauncherWindow *ui;
};

#endif