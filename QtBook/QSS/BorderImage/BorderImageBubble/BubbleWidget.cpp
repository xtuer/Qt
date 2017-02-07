#include "BubbleWidget.h"
#include "ui_BubbleWidget.h"

BubbleWidget::BubbleWidget(QWidget *parent) : QWidget(parent), ui(new Ui::BubbleWidget) {
    ui->setupUi(this);
}

BubbleWidget::~BubbleWidget() {
    delete ui;
}
