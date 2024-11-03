#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include<QLabel>
#include <QLineEdit>
#include<QPushButton>


namespace Ui {
class CLoginDialog;
}

class CLoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CLoginDialog(QWidget *parent = nullptr);
    ~CLoginDialog();

public slots:
    void login();
    //void openRegisterDialog();

private:
    Ui::CLoginDialog *ui;
    QLineEdit* usernameEdit;
    QLineEdit* passwordEdit;
    QPushButton *loginButton;
    QPushButton *registerButton;


};

#endif // LOGINDIALOG_H
