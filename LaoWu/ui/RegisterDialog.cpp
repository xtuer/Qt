#include "RegisterDialog.h"
#include "ui_RegisterDialog.h"

#include "../sn/Register.h"
#include "../sn/RegisterFactory.h"

#include <QMessageBox>
#include <QCloseEvent>

RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::RegisterDialog) {
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    layout()->setSizeConstraint(QLayout::SetFixedSize);

    connect(ui->registerButton, SIGNAL(clicked()), this, SLOT(registerSN()));
}

RegisterDialog::~RegisterDialog() {
    delete ui;

    // 如果过期了, 则退出, 不可使用.
    if (RegisterFactory::getRegister().isExpired()) {
        QMessageBox::critical(this, tr("注册"), tr("软件已经过期, 如需继续使用, 请注册!"));

        exit(1);
    }
}

void RegisterDialog::checkRegister() {
    Register &r = RegisterFactory::getRegister();
    r.usedAgain();

    // 没有注册, 则提示注册.
    if (!r.isRegistered()) {
        ui->machineCodeEdit->setText(r.getMachineCode());
        ui->infoLabel->setText(r.getRegisterInfomation());

        exec();
    }
}

void RegisterDialog::registerSN() {
    QString sn = ui->snEdit->text().trimmed();

    if (RegisterFactory::getRegister().registerSN(sn)) {
        QMessageBox::information(this, tr("注册"), tr("注册成功"));
    } else {
        QMessageBox::information(this, tr("注册"), tr("注册失败"));
    }
}
 
