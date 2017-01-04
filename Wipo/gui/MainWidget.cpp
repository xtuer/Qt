#include "MainWidget.h"
#include "ui_MainWidget.h"
#include "CentralWidget.h"
#include "IntroductionWidget.h"

#include <QPainter>
#include <QPixmap>
#include <QMouseEvent>

MainWidget::MainWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MainWidget) {
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_StyledBackground);

    layout()->addWidget(new CentralWidget);
    background.load(":/img/background.png");

    introductionWidget = new IntroductionWidget(this);
    introductionWidget->move(7, 64);

    connect(ui->closeButton, &QPushButton::clicked, [=] {
        close();
    });
}

MainWidget::~MainWidget() {
    delete ui;
}

void MainWidget::mousePressEvent(QMouseEvent *e) {
    mousePressedPosition = e->globalPos();
    windowPositionAsMousePressed = this->pos();
}

void MainWidget::mouseReleaseEvent(QMouseEvent *) {
    mousePressedPosition = QPoint();
}

void MainWidget::mouseMoveEvent(QMouseEvent *e) {
    if (mousePressedPosition.isNull()) { return; }

    QPoint delta = e->globalPos() - mousePressedPosition;
    QPoint pos = windowPositionAsMousePressed + delta;
    this->move(pos);
}

void MainWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, background);
}
