#include "logindialog.h"
#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Draw_Tool_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    CLoginDialog dtest;
    if (dtest.exec() == QDialog::Accepted) {
        MainWindow w;
        w.show();// 登录成功，进入主界面
        return a.exec();
    } else {
        return -1;// 登录失败或取消
    }
    // MainWindow w;
    // w.show();
    // return a.exec();
}
