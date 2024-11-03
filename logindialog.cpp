#include "logindialog.h"
#include "ui_logindialog.h"
#include <QVBoxLayout>
#include <qmessagebox.h>
#include"tool.h"
#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif

CLoginDialog::CLoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CLoginDialog)
{
    ui->setupUi(this);
     //setWindowTitle("登录");
     setWindowIcon(QIcon(":/drawicon.ico"));
    // 创建 UI 组件
    usernameEdit = new QLineEdit(this);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);  // 密码隐藏
    loginButton = new QPushButton("登录", this);
    registerButton = new QPushButton("注册", this);

    QVBoxLayout *layout = new QVBoxLayout(this);//垂直布局
    layout->addWidget(usernameEdit);
    layout->addWidget(passwordEdit);
    layout->addWidget(loginButton);
    layout->addWidget(registerButton);

    connect(loginButton, &QPushButton::clicked, this, &CLoginDialog::login);
    //connect(registerButton, &QPushButton::clicked, this, &CLoginDialog::registerUser);




}

CLoginDialog::~CLoginDialog()
{
    delete ui;
}

void CLoginDialog::login() {

    CTool::ConnectDB();
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();

    // 验证用户名和密码
    QSqlQuery query;
    query.prepare("SELECT * FROM user WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    if (query.exec() && query.next()) {
        accept();  // 登录成功，关闭对话框
    } else {
        QMessageBox::warning(this, "登录失败", "用户名或密码错误！");
    }
}
