/********************************************************************************
** Form generated from reading UI file 'materialscanner.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MATERIALSCANNER_H
#define UI_MATERIALSCANNER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_materialscannerClass
{
public:

    void setupUi(QWidget *materialscannerClass)
    {
        if (materialscannerClass->objectName().isEmpty())
            materialscannerClass->setObjectName(QString::fromUtf8("materialscannerClass"));
        materialscannerClass->resize(600, 400);

        retranslateUi(materialscannerClass);

        QMetaObject::connectSlotsByName(materialscannerClass);
    } // setupUi

    void retranslateUi(QWidget *materialscannerClass)
    {
        materialscannerClass->setWindowTitle(QCoreApplication::translate("materialscannerClass", "materialscanner", nullptr));
    } // retranslateUi

};

namespace Ui {
    class materialscannerClass: public Ui_materialscannerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MATERIALSCANNER_H
