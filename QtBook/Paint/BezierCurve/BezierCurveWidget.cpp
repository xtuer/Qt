#include "BezierCurveWidget.h"
#include <QPainter>
#include <QMouseEvent>
BezierCurveWidget::BezierCurveWidget(QWidget *parent) : QWidget(parent) {
    // 贝塞尔曲线的端点
    breakPoints.append(new QPointF(0, 0));
    breakPoints.append(new QPointF(100, 100));
    breakPoints.append(new QPointF(200, 0));
    breakPoints.append(new QPointF(300, 100));

    // 第一段贝塞尔曲线控制点
    controlPoints.append(new QPointF(50, 0));
    controlPoints.append(new QPointF(50, 100));

    // 第二段贝塞尔曲线控制点
    controlPoints.append(new QPointF(150, 100));
    controlPoints.append(new QPointF(150, 0));

    // 第三段贝塞尔曲线控制点
    controlPoints.append(new QPointF(250, 0));
    controlPoints.append(new QPointF(250, 100));

    // 坐标设置好后就可以生成贝塞尔曲线了
    bezierCurve = createBezierCurve();

    controlPointRadius = 8;
    translatedX = 50;
    translatedY = 50;
    flags = Qt::AlignHCenter | Qt::AlignVCenter; // 水平和垂直居中
}

BezierCurveWidget::~BezierCurveWidget() {
    qDeleteAll(breakPoints);
    qDeleteAll(controlPoints);
}

void BezierCurveWidget::mousePressEvent(QMouseEvent *event) {
    pressedControlPointIndex = -1;

    // 绘制贝塞尔曲线和控制点的坐标系做了偏移，鼠标按下的坐标也要相应的偏移
    QPointF p = translatedPoint(event->pos());

    // 鼠标按下时，选择被按住的控制点
    for (int i = 0; i < controlPoints.size(); ++i) {
        QPainterPath path;
        path.addEllipse(*controlPoints.at(i), controlPointRadius, controlPointRadius);

        if (path.contains(p)) {
            pressedControlPointIndex = i;
            break;
        }
    }
}

void BezierCurveWidget::mouseMoveEvent(QMouseEvent *event) {
    // 移动选中的控制点
    if (pressedControlPointIndex != -1) {
        QPointF p = translatedPoint(event->pos());
        controlPoints.at(pressedControlPointIndex)->setX(p.x());
        controlPoints.at(pressedControlPointIndex)->setY(p.y());
        bezierCurve = createBezierCurve(); // 坐标发生变化后重新生成贝塞尔曲线
        update(); // 刷新界面
    }
}

QPainterPath BezierCurveWidget::createBezierCurve() {
    QPainterPath curve;
    curve.moveTo(*breakPoints.at(0));
    curve.cubicTo(*controlPoints[0], *controlPoints[1], *breakPoints[1]);
    curve.cubicTo(*controlPoints[2], *controlPoints[3], *breakPoints[2]);
    curve.cubicTo(*controlPoints[4], *controlPoints[5], *breakPoints[3]);

    return curve;
}

QRect BezierCurveWidget::createControlPointBundingRect(int index) {
    int x = controlPoints.at(index)->x() - controlPointRadius;
    int y = controlPoints.at(index)->y() - controlPointRadius;
    return QRect(x, y, controlPointRadius * 2, controlPointRadius * 2);
}

QPointF BezierCurveWidget::translatedPoint(const QPointF &point) const {
    return point - QPointF(translatedX, translatedY);
}

void BezierCurveWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(translatedX, translatedY);

    // 绘制贝塞尔曲线
    painter.drawPath(bezierCurve);

    // 绘制控制点和控制点的序号
    painter.setBrush(Qt::lightGray);
    for (int i = 0; i < controlPoints.size(); ++i) {
        QRect rect = createControlPointBundingRect(i);
        painter.drawEllipse(rect);
        painter.drawText(rect, flags, QString("%1").arg(i));
    }
}
