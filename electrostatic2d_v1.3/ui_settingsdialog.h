/********************************************************************************
** Form generated from reading UI file 'settingsdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSDIALOG_H
#define UI_SETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingsDialog
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *layoutWidget;
    QFormLayout *formLayout;
    QLabel *label;
    QSpinBox *spinWidth;
    QLabel *label_2;
    QSpinBox *spinHeight;
    QLabel *label_3;
    QDoubleSpinBox *spinGridSize;

    void setupUi(QDialog *SettingsDialog)
    {
        if (SettingsDialog->objectName().isEmpty())
            SettingsDialog->setObjectName(QString::fromUtf8("SettingsDialog"));
        SettingsDialog->resize(400, 300);
        buttonBox = new QDialogButtonBox(SettingsDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(30, 240, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        layoutWidget = new QWidget(SettingsDialog);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 20, 350, 200));
        formLayout = new QFormLayout(layoutWidget);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        spinWidth = new QSpinBox(layoutWidget);
        spinWidth->setObjectName(QString::fromUtf8("spinWidth"));
        spinWidth->setMinimum(500);
        spinWidth->setMaximum(5000);
        spinWidth->setValue(2000);

        formLayout->setWidget(0, QFormLayout::FieldRole, spinWidth);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        spinHeight = new QSpinBox(layoutWidget);
        spinHeight->setObjectName(QString::fromUtf8("spinHeight"));
        spinHeight->setMinimum(500);
        spinHeight->setMaximum(5000);
        spinHeight->setValue(2000);

        formLayout->setWidget(1, QFormLayout::FieldRole, spinHeight);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        spinGridSize = new QDoubleSpinBox(layoutWidget);
        spinGridSize->setObjectName(QString::fromUtf8("spinGridSize"));
        spinGridSize->setMinimum(5.000000000000000);
        spinGridSize->setMaximum(100.000000000000000);
        spinGridSize->setValue(20.000000000000000);

        formLayout->setWidget(2, QFormLayout::FieldRole, spinGridSize);


        retranslateUi(SettingsDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), SettingsDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), SettingsDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(SettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *SettingsDialog)
    {
        SettingsDialog->setWindowTitle(QCoreApplication::translate("SettingsDialog", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270", nullptr));
        label->setText(QCoreApplication::translate("SettingsDialog", "\320\250\320\270\321\200\320\270\320\275\320\260:", nullptr));
        label_2->setText(QCoreApplication::translate("SettingsDialog", "\320\222\321\213\321\201\320\276\321\202\320\260:", nullptr));
        label_3->setText(QCoreApplication::translate("SettingsDialog", "\320\240\320\260\320\267\320\274\320\265\321\200 \321\201\320\265\321\202\320\272\320\270:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SettingsDialog: public Ui_SettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSDIALOG_H
