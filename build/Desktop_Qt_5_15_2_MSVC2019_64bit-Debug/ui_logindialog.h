/********************************************************************************
** Form generated from reading UI file 'logindialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINDIALOG_H
#define UI_LOGINDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_CLoginDialog
{
public:

    void setupUi(QDialog *CLoginDialog)
    {
        if (CLoginDialog->objectName().isEmpty())
            CLoginDialog->setObjectName(QString::fromUtf8("CLoginDialog"));
        CLoginDialog->resize(400, 300);

        retranslateUi(CLoginDialog);

        QMetaObject::connectSlotsByName(CLoginDialog);
    } // setupUi

    void retranslateUi(QDialog *CLoginDialog)
    {
        CLoginDialog->setWindowTitle(QCoreApplication::translate("CLoginDialog", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CLoginDialog: public Ui_CLoginDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINDIALOG_H
