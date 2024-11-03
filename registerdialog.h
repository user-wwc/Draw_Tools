#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

class RegisterDialog : public QDialog {
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);

public slots:
    void registerUser();

private:
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QLineEdit *confirmPasswordEdit;
    QPushButton *registerButton;
};

#endif // REGISTERDIALOG_H

