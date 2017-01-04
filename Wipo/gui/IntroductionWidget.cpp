#include "IntroductionWidget.h"
#include "ui_IntroductionWidget.h"
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QGraphicsOpacityEffect>

IntroductionWidget::IntroductionWidget(QWidget *parent) : QWidget(parent), ui(new Ui::IntroductionWidget) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);

    connect(ui->startButton, &QPushButton::clicked, [=] {
        QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
        effect->setOpacity(1);
        setGraphicsEffect(effect);

        QPropertyAnimation *an = new QPropertyAnimation(effect, "opacity");
        an->setEndValue(0);
        an->setDuration(300);
        an->start(QAbstractAnimation::DeleteWhenStopped);
        an->setEasingCurve(QEasingCurve::InCubic);

        connect(an, &QPropertyAnimation::finished, [=] {
            hide();
        });
    });
}

IntroductionWidget::~IntroductionWidget() {
    delete ui;
}

void IntroductionWidget::showEvent(QShowEvent *) {
    int y = 233;

    QPropertyAnimation *an1 = new QPropertyAnimation(ui->flashWidget, "pos");
    an1->setStartValue(QPoint(-1000, y));
    an1->setEndValue(QPoint(1500, y));
    an1->setDuration(500);
    an1->setEasingCurve(QEasingCurve::InCubic);

    QPropertyAnimation *an2 = new QPropertyAnimation(ui->flashWidget, "pos");
    an2->setEndValue(QPoint(0, y));
    an2->setDuration(500);
    an2->setEasingCurve(QEasingCurve::OutCubic);

    QSequentialAnimationGroup *anGroup = new QSequentialAnimationGroup();
    anGroup->addAnimation(an1);
//    anGroup->addAnimation(an2);
    anGroup->start(QAbstractAnimation::DeleteWhenStopped);
}
