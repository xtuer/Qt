#include "Widget.h"
#include <QGridLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QDebug>

Widget::Widget(QWidget *parent) : QWidget(parent) {
    const int     buttonsCount = 1000; // 按钮的数量
    QGridLayout *buttonsLayout = new QGridLayout();

    for (int i = 0; i < buttonsCount; ++i) {
        // 每行显示 10 个
        int row = i / 10;
        int col = i % 10;
        QPushButton *button = new QPushButton(QString("%1-%2").arg(row+1).arg(col+1));
        buttonsLayout->addWidget(button, row, col);

        connect(button, SIGNAL(clicked()), this, SLOT(buttonClicked()));
    }

    // 把按钮全添加到 buttonsContainer 中，然后把 buttonsContainer 再加到 scrollArea 上
    QWidget *buttonsContainer = new QWidget();
    buttonsContainer->setLayout(buttonsLayout);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(buttonsContainer);

    QGridLayout *mainLayout = new QGridLayout();
    mainLayout->addWidget(scrollArea);
    this->setLayout(mainLayout);
}

Widget::~Widget() {
}

void Widget::buttonClicked() {
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    qDebug() << button->text();
}
