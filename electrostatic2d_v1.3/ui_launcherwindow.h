/********************************************************************************
** Form generated from reading UI file 'launcherwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LAUNCHERWINDOW_H
#define UI_LAUNCHERWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LauncherWindow
{
public:
    QPushButton *btnNew;
    QPushButton *btnOpen;

    void setupUi(QWidget *LauncherWindow)
    {
        if (LauncherWindow->objectName().isEmpty())
            LauncherWindow->setObjectName(QString::fromUtf8("LauncherWindow"));
        LauncherWindow->resize(400, 300);
        btnNew = new QPushButton(LauncherWindow);
        btnNew->setObjectName(QString::fromUtf8("btnNew"));
        btnNew->setGeometry(QRect(100, 100, 200, 40));
        btnOpen = new QPushButton(LauncherWindow);
        btnOpen->setObjectName(QString::fromUtf8("btnOpen"));
        btnOpen->setGeometry(QRect(100, 160, 200, 40));

        retranslateUi(LauncherWindow);

        QMetaObject::connectSlotsByName(LauncherWindow);
    } // setupUi

    void retranslateUi(QWidget *LauncherWindow)
    {
        LauncherWindow->setWindowTitle(QCoreApplication::translate("LauncherWindow", "Electrostatic2D", nullptr));
        btnNew->setText(QCoreApplication::translate("LauncherWindow", "\320\235\320\276\320\262\321\213\320\271 \320\277\321\200\320\276\320\265\320\272\321\202", nullptr));
        btnOpen->setText(QCoreApplication::translate("LauncherWindow", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214 \320\277\321\200\320\276\320\265\320\272\321\202", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LauncherWindow: public Ui_LauncherWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LAUNCHERWINDOW_H
