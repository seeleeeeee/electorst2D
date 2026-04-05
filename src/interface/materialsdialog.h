#ifndef MATERIALSDIALOG_H
#define MATERIALSDIALOG_H

#include <QDialog>
#include "../structures/electrostatic_structures.h"

namespace Ui {
class MaterialsDialog;
}

class MaterialsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MaterialsDialog(QWidget *parent = nullptr);
    ~MaterialsDialog();

    void setMaterials(const electrostatic::Materials& materials);
    electrostatic::Materials getMaterials() const;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::MaterialsDialog *ui;
    electrostatic::Materials currentMaterials;
};

#endif