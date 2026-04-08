#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_buttonBox_accepted()
{
    int width = ui->spinWidth ? ui->spinWidth->value() : 2000;
    int height = ui->spinHeight ? ui->spinHeight->value() : 2000;
    double gridSize = ui->spinGridSize ? ui->spinGridSize->value() : 20.0;
    emit settingsChanged(width, height, gridSize);
    accept();
}