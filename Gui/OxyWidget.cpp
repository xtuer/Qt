#include "OxyWidget.h"
#include "ui_OxyWidget.h"

OxyWidget::OxyWidget(QWidget *parent) : QWidget(parent),
    ui(new Ui::OxyWidget) {
    ui->setupUi(this);
//    setStyleSheet("background: red;");
//    setAttribute(Qt::WA_StyledBackground, true); // [[1]]
}

OxyWidget::~OxyWidget() {
    delete ui;
}
