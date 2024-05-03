#include "SlotWarning.h"
#include "ui_SlotWarning.h"

#include <QMouseEvent>

SlotWarning::SlotWarning(QWidget *parent): QWidget(parent), ui(new Ui::SlotWarning) {
    ui->setupUi(this);
    ui->warningWidget->installEventFilter(this);
}

SlotWarning::~SlotWarning() {
    delete ui;
}

bool SlotWarning::eventFilter(QObject *obj, QEvent *event) {
    // 点击后发送 emitBack 信号。
    if (obj == ui->warningWidget && event->type() == QEvent::MouseButtonPress) {
        emit emitBack();

        return true;
    }

    return QObject::eventFilter(obj, event);
}
