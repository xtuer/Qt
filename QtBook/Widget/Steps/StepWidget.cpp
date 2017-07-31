#include "StepWidget.h"
#include "UiUtil.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QShortcut>
#include <QKeySequence>
#include <QDebug>

StepWidget::StepWidget(QWidget *parent) : QWidget(parent) {
    QHBoxLayout *layout = new QHBoxLayout();
    QStringList steps = QStringList() << "提交订单" << "付款成功" << "商品出库" << "------ 快递中-等待收货 ------" << "完成";

    // 创建 steps.size() 个按钮，每个按钮表示一个步骤
    foreach (const QString &step, steps) {
        QPushButton *button = new QPushButton(step);
        button->setFlat(true);
        stepButtons.append(button);
        layout->addWidget(button);

        connect(button, SIGNAL(clicked()), this, SLOT(updateStepButtonsStyle()));
    }

    layout->addStretch();
    layout->setSpacing(0);
    setLayout(layout);

    // 初始化按钮的状态，有 5 个状态: middle, last, active-middle, active-last, active-prev
    foreach (QPushButton *b, stepButtons) {
        b->setProperty("class", "StepButton");
        b->setProperty("status", "middle");
    }
    stepButtons.last()->setProperty("status", "last");
    stepButtons.at(2)->click();

    createLoadQssAction();
}

StepWidget::~StepWidget() {
}

void StepWidget::updateStepButtonsStyle() {
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender()); // 被点击的按钮
    int clickedIndex = stepButtons.indexOf(clickedButton);
    QPushButton *prevButton = stepButtons.value(clickedIndex - 1); // 被点击的按钮的前一个按钮
    bool isLast = (clickedIndex == stepButtons.size() - 1); // 是否点击最后一个按钮

    // 恢复所有按钮的 status 为初始状态
    foreach (QPushButton *b, stepButtons) {
        b->setProperty("status", "middle");
    }
    stepButtons.last()->setProperty("status", "last");

    // 设置被点击的按钮的 status 属性为 active，如果是最后一个按钮则为 active-last
    clickedButton->setProperty("status", isLast ? "active-last" : "active-middle");

    // 设置被点击的按钮的前一个按钮的 status 为 active-prev
    prevButton && prevButton->setProperty("status", "active-prev");

    // 属性变化后，使属性选择器的 QSS 生效
    UiUtil::updateQss(this);
}

void StepWidget::createLoadQssAction() {
    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_L), this);

    connect(shortcut, &QShortcut::activated, [] {
        UiUtil::loadQss();
    });
}
