#include "CheckedButton.h"
#include "ui_CheckedButton.h"

CheckedButton::CheckedButton(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CheckedButton) {
    ui->setupUi(this);
    this->setAttribute(Qt::WA_StyledBackground); // 启用 QSS
}

CheckedButton::~CheckedButton() {
    delete ui;
}

void CheckedButton::mousePressEvent(QMouseEvent *event) {
    this->setChecked(!this->checked);
    emit this->clicked(this->checked);
}
