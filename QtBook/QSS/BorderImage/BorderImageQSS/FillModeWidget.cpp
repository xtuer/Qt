#include "FillModeWidget.h"
#include "ui_FillModeWidget.h"

FillModeWidget::FillModeWidget(QWidget *parent) : QWidget(parent), ui(new Ui::FillModeWidget) {
    ui->setupUi(this);
}

FillModeWidget::~FillModeWidget() {
    delete ui;
}
