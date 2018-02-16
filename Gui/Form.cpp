#include "Form.h"
#include "ui_Form.h"

#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsBlurEffect>

Form::Form(QWidget *parent) : QWidget(parent), ui(new Ui::Form) {
    ui->setupUi(this);

    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setColor(Qt::darkRed);
    shadowEffect->setOffset(0, 0);
    shadowEffect->setBlurRadius(30);
    ui->shadowLabel->setGraphicsEffect(shadowEffect);

    QGraphicsBlurEffect *blurEffect = new QGraphicsBlurEffect(this);
    blurEffect->setBlurRadius(6);
    blurEffect->setBlurHints(QGraphicsBlurEffect::QualityHint);
    ui->blurLabel->setGraphicsEffect(blurEffect);
}
