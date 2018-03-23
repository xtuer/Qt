#include "MyWidget.h"
#include "ui_MyWidget.h"

#include <QDebug>
#include <QAction>
#include <QActionGroup>
#include <QMenu>
#include <QContextMenuEvent>

MyWidget::MyWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MyWidget) {
    initializeUi();
    handleEvents();
}

MyWidget::~MyWidget() {
    delete ui;
}

bool MyWidget::eventFilter(QObject *watched, QEvent *event) {
    return QWidget::eventFilter(watched, event);
}

void MyWidget::contextMenuEvent(QContextMenuEvent *event) {
    QAction *javaAction = new QAction("Java", this);
    QAction *cppAction  = new QAction("C++", this);
    QAction *phpAction  = new QAction("PHP", this);
    QAction *jsAction   = new QAction("JavaScript", this);

    connect(javaAction, &QAction::triggered, [] {
        qDebug() << "点击菜单项";
    });

    QMenu menu;
    menu.addAction(javaAction);
    menu.addAction(cppAction);
    menu.addAction(phpAction);
    menu.addAction(jsAction);
    menu.exec(mapToGlobal(event->pos()));
}

void MyWidget::initializeUi() {
    ui->setupUi(this);
}

void MyWidget::handleEvents() {

}
