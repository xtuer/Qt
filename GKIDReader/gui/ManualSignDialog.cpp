#include "ManualSignDialog.h"
#include "ui_ManualSignDialog.h"

#include <QMessageBox>

ManualSignDialog::ManualSignDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManualSignDialog) {
    ui->setupUi(this);

    connect(ui->cancelButton, &QPushButton::clicked, [this] {
        this->reject();
    });

    connect(ui->signButton, &QPushButton::clicked, [this] {
       this->name = ui->nameLineEdit->text().trimmed();
       this->examNum = ui->examNumLineEdit->text().trimmed();
       this->signCode = ui->signCodeLineEdit->text().trimmed();

       if (name.isEmpty() || examNum.isEmpty() || signCode.isEmpty()) {
           QMessageBox::warning(nullptr, "错误", "请填写完整");
           return;
       } else {
           this->accept();
       }
    });
}

ManualSignDialog::~ManualSignDialog() {
    delete ui;
}
