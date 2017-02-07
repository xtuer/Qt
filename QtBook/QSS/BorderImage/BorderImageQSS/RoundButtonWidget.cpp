#include "RoundButtonWidget.h"
#include "ui_RoundButtonWidget.h"

RoundButtonWidget::RoundButtonWidget(QWidget *parent) : QWidget(parent), ui(new Ui::RoundButtonWidget) {
    ui->setupUi(this);
}

RoundButtonWidget::~RoundButtonWidget() {
    delete ui;
}
