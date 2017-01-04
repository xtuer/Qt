#include "IntroductionWidget.h"
#include "ui_IntroductionWidget.h"
#include "TitleLabel.h"

#include <QDebug>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsOpacityEffect>

IntroductionWidget::IntroductionWidget(QWidget *parent) : QWidget(parent), ui(new Ui::IntroductionWidget) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);

    titleLabel = new TitleLabel(this);
    titleLabel->move(-5000, 230);

    connect(ui->startButton, &QPushButton::clicked, [=] {
        QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
        effect->setOpacity(1);
        this->setGraphicsEffect(effect);

        QPropertyAnimation *an = new QPropertyAnimation(effect, "opacity");
        an->setEndValue(0);
        an->setDuration(300);
        an->setEasingCurve(QEasingCurve::InCubic);
        an->start(QAbstractAnimation::DeleteWhenStopped);

        connect(an, &QPropertyAnimation::finished, [=] {
            this->hide();
        });
    });
}

IntroductionWidget::~IntroductionWidget() {
    delete ui;
}

void IntroductionWidget::showEvent(QShowEvent *) {
    QPropertyAnimation *an = new QPropertyAnimation(titleLabel, "geometry");
    an->setStartValue(QRect(470, 280, 1, 1));
    an->setEndValue(QRect(10, 210, 945, 161));
    an->setDuration(2000);
    an->setEasingCurve(QEasingCurve::OutBounce);
    an->start(QAbstractAnimation::DeleteWhenStopped);

//    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
//    effect->setOpacity(0);
//    titleLabel->setGraphicsEffect(effect);

//    QPropertyAnimation *effectAn = new QPropertyAnimation(titleLabel, "opacity");
//    effectAn->setStartValue(0);
//    effectAn->setEndValue(1);
//    effectAn->setDuration(1000);
//    effectAn->setEasingCurve(QEasingCurve::InCubic);

//    QParallelAnimationGroup *anGroup = new QParallelAnimationGroup(this);
//    anGroup->addAnimation(an);
//    anGroup->addAnimation(effectAn);
//    anGroup->start(QAbstractAnimation::DeleteWhenStopped);
}
