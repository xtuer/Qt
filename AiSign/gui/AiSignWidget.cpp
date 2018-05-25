#include "AiSignWidget.h"
#include "ui_AiSignWidget.h"

AiSignWidget::AiSignWidget(QWidget *parent) : QWidget(parent), ui(new Ui::AiSignWidget) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);
}

AiSignWidget::~AiSignWidget() {
    delete ui;
}
