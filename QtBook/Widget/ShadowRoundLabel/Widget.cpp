#include "Widget.h"
#include "ui_Widget.h"
#include <QGraphicsDropShadowEffect>
#include <QGraphicsBlurEffect>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);

    // 阴影效果
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setColor(Qt::darkRed);
    shadowEffect->setOffset(0, 0);
    shadowEffect->setBlurRadius(30);
    ui->shadowLabel->setGraphicsEffect(shadowEffect);

    // 模糊效果
    QGraphicsBlurEffect *blurEffect = new QGraphicsBlurEffect(this);
    blurEffect->setBlurRadius(6);
    blurEffect->setBlurHints(QGraphicsBlurEffect::QualityHint);
    ui->blurLabel->setGraphicsEffect(blurEffect);

    QGraphicsBlurEffect *borderBlurEffect = new QGraphicsBlurEffect(this);
    borderBlurEffect->setBlurRadius(4);
    borderBlurEffect->setBlurHints(QGraphicsBlurEffect::QualityHint);

    // 第 5 个图标增加模糊的边框
    QWidget *onlyBorderWidget = new QWidget();
    onlyBorderWidget->setProperty("class", "BorderWidget");
    onlyBorderWidget->setGraphicsEffect(borderBlurEffect);

    QGridLayout *l = qobject_cast<QGridLayout*>(ui->widget->layout());
    l->addWidget(onlyBorderWidget, 0, 0, 1, 1, Qt::AlignHCenter | Qt::AlignVCenter);
    l->setAlignment(ui->innerLabel, Qt::AlignHCenter | Qt::AlignVCenter);
}

Widget::~Widget() {
    delete ui;
}
