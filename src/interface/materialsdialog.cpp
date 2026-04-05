#include "materialsdialog.h"
#include "ui_materialsdialog.h"

MaterialsDialog::MaterialsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MaterialsDialog)
{
    ui->setupUi(this);
}

MaterialsDialog::~MaterialsDialog()
{
    delete ui;
}

void MaterialsDialog::setMaterials(const electrostatic::Materials& materials)
{
    currentMaterials = materials;
}

electrostatic::Materials MaterialsDialog::getMaterials() const
{
    return currentMaterials;
}

void MaterialsDialog::on_buttonBox_accepted()
{
    accept();
}