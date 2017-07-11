#include "Form.h"
#include "SmoothCurveGenerator.h"

#include <QDebug>
#include <QPainter>
#include <QDateTime>

Form::Form(QWidget *parent) : QWidget(parent) {
    qsrand(QDateTime::currentDateTime().toTime_t());

    // 随机生成曲线第一条曲线的坐标
    int x = 0, y = 0;
    for (int i = 0; i < 100; ++i) {
        x += qrand() % 30 + 20;
        y = qrand() % 180 + 30;

//        points1 << QPointF(x, y);
    }

    points1 << QPointF(0, 0) << QPointF(50, 100) << QPointF(100, 30) << QPointF(120, 0) << QPointF(150, 100);

    // 第二条星行曲线的坐标
    points2 << QPointF(0, 150) << QPointF(50, 50) << QPointF(150, 0) << QPointF(50, -50)
            << QPointF(0, -150) << QPointF(-50, -50) << QPointF(-150, 0) << QPointF(-50, 50);

    // 使用曲线的坐标生成平滑曲线
    smoothCurvePath1 = SmoothCurveGenerator::generateSmoothCurve(points1); // 第一条曲线不封闭
    smoothCurvePath2 = SmoothCurveGenerator::generateSmoothCurve(points2, true); // 第二条曲线是封闭的
}

Form::~Form() {
}


void Form::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::black, 2));


    painter.translate(0, 150);
    painter.scale(1, -1);
    // 绘制第一条平滑曲线和曲线上的顶点
    painter.drawPath(smoothCurvePath1);
    painter.setBrush(Qt::gray);
    for (int i = 0; i < points1.length() ; i += 1) {
        painter.drawEllipse(points1[i].x()-3, points1[i].y()-3, 6, 6);
    }

    // 绘制第二条平滑曲线和曲线上的顶点
    painter.translate(200, 400);
    painter.drawPath(smoothCurvePath2);
    for (int i = 0; i < points2.length() ; i += 1) {
        painter.drawEllipse(points2[i].x()-3, points2[i].y()-3, 6, 6);
    }
}

