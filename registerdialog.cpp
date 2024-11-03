#include "registerdialog.h"

#include "tool.h"
#include "registerdialog.h"
#include <QDialog>
#include <QSqlQuery>
#include <QMessageBox>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif
RegisterDialog::RegisterDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("注册");
 setWindowIcon(QIcon(":/drawicon.ico"));
    // 创建 UI 组件
    usernameEdit = new QLineEdit(this);
    passwordEdit = new QLineEdit(this);
    confirmPasswordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);
    confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    registerButton = new QPushButton("注册", this);

    // 布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(usernameEdit);
    layout->addWidget(passwordEdit);
    layout->addWidget(confirmPasswordEdit);
    layout->addWidget(registerButton);

    // 连接按钮信号与槽
    connect(registerButton, &QPushButton::clicked, this, &RegisterDialog::registerUser);
}


void RegisterDialog::registerUser() {
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();
    QString confirmPassword = confirmPasswordEdit->text();

    if (password == confirmPassword) {
        CTool::ConnectDB();
        QSqlQuery query;
        query.prepare("INSERT INTO user (username, password) VALUES (:username, :password)");
        query.bindValue(":username", username);
        query.bindValue(":password", password);

        if (query.exec()) {
            QMessageBox::information(this, "注册成功", "注册成功，请登录！");
            accept();  // 注册成功，关闭对话框
        } else {
            QMessageBox::warning(this, "注册失败", "注册失败，请重试！");
        }
    } else {
        QMessageBox::warning(this, "注册失败", "密码不一致！");
    }
}



