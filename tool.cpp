#include "tool.h"
#include<QMessageBox>
#include<QDebug>
#include<QSqlError>
CTool::CTool() {}

 void CTool::ConnectDB(){
      QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("aa");
    db.setUserName("root");  // 请根据你的设置更改用户名
    db.setPassword("123456");  // 请根据你的设置更改密码
    bool ok = db.open();
    if (ok){
        QMessageBox::information(NULL, "infor", "success");
    }
    else {
        QMessageBox::information(NULL, "infor", "open failed");
        qDebug()<<"error open database because"<<db.lastError().text();
        //qDebug() << QSqlDatabase::drivers();
    }

}
