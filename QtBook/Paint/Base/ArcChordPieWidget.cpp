#include "ArcChordPieWidget.h"
#include "ui_ArcChordPieWidget.h"
#include <QPainter>

ArcChordPieWidget::ArcChordPieWidget(QWidget *parent) : QWidget(parent), ui(new Ui::ArcChordPieWidget) {
    ui->setupUi(this);
}

ArcChordPieWidget::~ArcChordPieWidget() {
    delete ui;
}

void ArcChordPieWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    static int startAngle = 45 * 16; // 开始角度是 45 度
    static int spanAngle = 130 * 16; // 覆盖角度为 130 度
    static QRectF boundingRect(0, 0, 150, 150); // 包围矩形

    painter.translate(30, 30);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(boundingRect); // 绘制包围矩形
    painter.setBrush(Qt::lightGray);
    painter.drawArc(boundingRect, startAngle, spanAngle); // 画弧

    painter.translate(180, 0);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(boundingRect); // 绘制包围矩形
    painter.setBrush(Qt::lightGray);
    painter.drawChord(boundingRect, startAngle, spanAngle); // 画弦

    painter.translate(180, 0);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(boundingRect); // 绘制包围矩形
    painter.setBrush(Qt::lightGray);
    painter.drawPie(boundingRect, startAngle, spanAngle); // 画饼图
}
