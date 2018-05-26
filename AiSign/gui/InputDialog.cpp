#include "InputDialog.h"
#include "ui_InputDialog.h"
#include <QMessageBox>

InputDialog::InputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputDialog) {
    ui->setupUi(this);
    setStyleSheet("QPushButton {min-width: 60px}");
    handleEvents();
}

InputDialog::~InputDialog() {
    delete ui;
}

QString InputDialog::getExamineeName() const {
    return examineeName;
}

QString InputDialog::getIdCardNo() const {
    return idCardNo;
}

void InputDialog::handleEvents() {
    connect(ui->signInButton, &QPushButton::clicked, [this] {
        examineeName = ui->nameLineEdit->text().trimmed();
        idCardNo = ui->idCardNoLineEdit->text().trimmed();

        if (examineeName.isEmpty()) {
            QMessageBox::warning(this, "错误", "考生姓名不能为空");
            return;
        }

        if (idCardNo.isEmpty()) {
            QMessageBox::warning(this, "错误", "证件号码不能为空");
            return;
        }

        accept();
    });
}
