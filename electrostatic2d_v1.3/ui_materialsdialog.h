/********************************************************************************
** Form generated from reading UI file 'materialsdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MATERIALSDIALOG_H
#define UI_MATERIALSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeWidget>

QT_BEGIN_NAMESPACE

class Ui_MaterialsDialog
{
public:
    QDialogButtonBox *buttonBox;
    QTreeWidget *treeWidget;

    void setupUi(QDialog *MaterialsDialog)
    {
        if (MaterialsDialog->objectName().isEmpty())
            MaterialsDialog->setObjectName(QString::fromUtf8("MaterialsDialog"));
        MaterialsDialog->resize(400, 300);
        buttonBox = new QDialogButtonBox(MaterialsDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(30, 240, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        treeWidget = new QTreeWidget(MaterialsDialog);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        treeWidget->setGeometry(QRect(10, 10, 380, 220));

        retranslateUi(MaterialsDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), MaterialsDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), MaterialsDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(MaterialsDialog);
    } // setupUi

    void retranslateUi(QDialog *MaterialsDialog)
    {
        MaterialsDialog->setWindowTitle(QCoreApplication::translate("MaterialsDialog", "\320\234\320\260\321\202\320\265\321\200\320\270\320\260\320\273\321\213", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(1, QCoreApplication::translate("MaterialsDialog", "\320\227\320\275\320\260\321\207\320\265\320\275\320\270\320\265", nullptr));
        ___qtreewidgetitem->setText(0, QCoreApplication::translate("MaterialsDialog", "\320\234\320\260\321\202\320\265\321\200\320\270\320\260\320\273", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MaterialsDialog: public Ui_MaterialsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MATERIALSDIALOG_H
