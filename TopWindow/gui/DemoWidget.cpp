#include "DemoWidget.h"
#include "ui_DemoWidget.h"

DemoWidget::DemoWidget(QWidget *parent) : QWidget(parent), ui(new Ui::DemoWidget) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);
    setStyleSheet("DemoWidget {background: white;} ");
}

DemoWidget::~DemoWidget() {
    delete ui;
}
