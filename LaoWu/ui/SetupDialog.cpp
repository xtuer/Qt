#include "SetupDialog.h"
#include "ui_SetupDialog.h"
#include "../util/Settings.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QCryptographicHash>

SetupDialog::SetupDialog(QWidget *parent) : QDialog(parent), ui(new Ui::SetupDialog) {
    ui->setupUi(this);

    setWindowFlags(Qt::WindowCloseButtonHint);
    ui->pathLineEdit->setText(Settings::getInstance().getBackupDirectory());

    connect(ui->browseButton,       SIGNAL(clicked()), this, SLOT(browse()));
    connect(ui->savePathButton,     SIGNAL(clicked()), this, SLOT(savePath()));
    connect(ui->savePasswordButton, SIGNAL(clicked()), this, SLOT(savePassword()));
}

SetupDialog::~SetupDialog() {
    delete ui;
}

void SetupDialog::browse() {
    QString backup = Settings::getInstance().getBackupDirectory();
    QString path = QFileDialog::getExistingDirectory(this, tr("选择备份目录"), backup);

    if (!path.isEmpty()) {
        ui->pathLineEdit->setText(path);
    }
}

void SetupDialog::savePassword() {
    QString password1 = ui->password1LineEdit->text();
    QString password2 = ui->password2LineEdit->text();

    if (password1 == password2) {
        QByteArray ba;
        ba.append(password1);
        QString md5Password = QCryptographicHash::hash(ba, QCryptographicHash::Md5).toHex().constData();
        Settings::getInstance().setPassword(md5Password);
    } else {
        QMessageBox::warning(this, tr("修改密码"), tr("密码不一致, 请重新输入"));
    }
}

void SetupDialog::savePath() {
    Settings::getInstance().setBackupDirectory(ui->pathLineEdit->text().trimmed());
}
