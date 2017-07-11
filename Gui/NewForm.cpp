#include "NewForm.h"
#include "ui_NewForm.h"
#include "DraggableLabel.h"

NewForm::NewForm(QWidget *parent) : QWidget(parent), ui(new Ui::NewForm) {
    ui->setupUi(this);
    DraggableLabel *label = new DraggableLabel("自己写的类，继承自 QLabel", this);
    label->move(20, 20);
    label->resize(200, 50);
}

NewForm::~NewForm() {
    delete ui;
}
