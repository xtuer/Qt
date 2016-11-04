#include "MainWidget.h"
#include "ui_MainWidget.h"

#include <QDebug>

MainWidget::MainWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MainWidget) {
    ui->setupUi(this);

    setAttribute(Qt::WA_StyledBackground, true);

    // 添加详情按钮
    QHBoxLayout *l = new QHBoxLayout();
    l->setContentsMargins(0,0,10,0);
    l->addStretch();
    l->addWidget(new QPushButton("详情"));
    ui->statisticLabel->setLayout(l);
}

MainWidget::~MainWidget() {
    delete ui;
}
