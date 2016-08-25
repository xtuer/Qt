#include "LoginDialog.h"
#include "ui_LoginDialog.h"

#include "../util/Settings.h"
#include <QMessageBox>
#include <QCryptographicHash>

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent), ui(new Ui::LoginDialog) {
    ui->setupUi(this);
    connect(ui->loginButton, SIGNAL(clicked()), this, SLOT(checkPassword()));
}

LoginDialog::~LoginDialog() {
    delete ui;
}

void LoginDialog::checkPassword() {
    QString password = Settings::getInstance().getPassword();

    QByteArray ba;
    ba.append(ui->passwordLineEdit->text());
    QString md5Password = QCryptographicHash::hash(ba, QCryptographicHash::Md5).toHex().constData();

    if (password == md5Password) {
        accept();
    } else {
        QMessageBox::information(this, tr("密码"), tr("密码不对"));
    }
}
