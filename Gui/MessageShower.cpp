#include "MessageShower.h"
#include "ui_MessageShower.h"
#include <QLabel>
#include <QPropertyAnimation>

MessageShower::MessageShower(QWidget *parent) : QWidget(parent), ui(new Ui::MessageShower) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);
    setStyleSheet(".MessageShower { background: white }");
}

void MessageShower::showMessage(const QString &text) {
    int w = ui->messageWidget->width();
    int h = ui->messageWidget->height();

    QLabel *messageLabel = new QLabel(text, ui->messageWidget);
//    messageLabel->setStyleSheet("border: 1px solid red");
    messageLabel->setGeometry(0, -200, w, h);
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->show();

    QPropertyAnimation *animation = new QPropertyAnimation(messageLabel, "pos");
    animation->setDuration(300);
    animation->setEndValue(QPoint(0, 0));

    connect(animation, &QPropertyAnimation::finished, [=] {
        if (lastMessage != NULL) {
            lastMessage->deleteLater();
        }

        lastMessage = messageLabel;
    });

    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

