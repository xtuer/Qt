#include "ReportWizardBase.h"
#include <QStackedWidget>
#include <QDebug>

ReportWizardBase::ReportWizardBase() {

}

ReportWizardBase::~ReportWizardBase() {

}

// 跳转到 wizard 的上一页, 如果是第一页返回 false (不能再往前跳转了), 否则返回 true
bool ReportWizardBase::prevPage() {
    if (nullptr == stackedWidget) {
        qDebug() << "还没有设置 stackedWidget";
        return false;
    }

    int current = stackedWidget->currentIndex();

    if (current > 0) {
        stackedWidget->setCurrentIndex(--current);
    }

    return current != 0;
}

// 跳转到 wizard 的下一页, 如果是末一页返回 false (不能再往后跳转了), 否则返回 true
bool ReportWizardBase::nextPage() {
    if (nullptr == stackedWidget) {
        qDebug() << "还没有设置 stackedWidget";
        return false;
    }

    int current = stackedWidget->currentIndex();
    int count   = stackedWidget->count();

    if (current < count) {
        stackedWidget->setCurrentIndex(++current);
    }

    return current != count - 1;
}

void ReportWizardBase::initialize(QStackedWidget *stackedWidget) {
    this->stackedWidget = stackedWidget;
}

