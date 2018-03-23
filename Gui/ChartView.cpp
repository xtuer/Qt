#include "ChartView.h"

#include <QDebug>
#include <QMouseEvent>
#include <QValueAxis>
#include <QLabel>

SelectableChartView::SelectableChartView(QChart *chart) : QChartView(chart) {
    setDragMode(QGraphicsView::RubberBandDrag);
    setRenderHint(QPainter::Antialiasing);
    chart->installEventFilter(this);
}

bool SelectableChartView::eventFilter(QObject *watched, QEvent *event) {
    // 在 QChart 上按下鼠标时保存按下的坐标
    if (watched == chart() && event->type() == QEvent::GraphicsSceneMousePress) {
        QMouseEvent *me = static_cast<QMouseEvent*>(event);
        pressedPointAtChart = me->pos();
    }

    return QChartView::eventFilter(watched, event);
}

void SelectableChartView::mouseReleaseEvent(QMouseEvent *event) {
    if (rubberBandRect().isEmpty()) { return; }

    // 注意这里用的是数值坐标，其他类型的坐标需要对应处理一下
    QValueAxis *axisX = qobject_cast<QValueAxis*>(chart()->axisX());
    QValueAxis *axisY = qobject_cast<QValueAxis*>(chart()->axisY());

    // QChartView 上的选区映射到 QChart 绘制曲线的矩形上, selectRect 的左下角和可视曲线的坐标轴上的原点重合
    QRectF plotRect = chart()->plotArea();
    QRectF selectRect(pressedPointAtChart - plotRect.topLeft(), QSize(rubberBandRect().size()));
    qDebug() << rubberBandRect();

    // 计算横坐标上的选区范围
    qreal minX   = axisX->min();
    qreal maxX   = axisX->max();
    qreal unitX  = (maxX-minX) / plotRect.width(); // 横轴上每个像素代表的数值
    qreal startX = minX   + unitX * selectRect.x();
    qreal endX   = startX + unitX * selectRect.width();

    // 计算纵坐标上的选区范围
    qreal minY   = axisY->min();
    qreal maxY   = axisY->max();
    qreal unitY  = (maxY-minY) / plotRect.height(); // 纵轴上每个像素代表的数值
    qreal startY = minY   + unitY * (plotRect.height() - selectRect.y() - selectRect.height());
    qreal endY   = startY + unitY * selectRect.height();

    // 创建选区 Label
    createSelectionLabel(startX, endX, startY, endY, rubberBandRect());

    // 让父类处理事件
    QChartView::mouseReleaseEvent(event);
}

QLabel* SelectableChartView::createSelectionLabel(qreal startX, qreal endX, qreal startY, qreal endY, QRect geometry) {
    QString text = QString("X: [%1, %2]\nY: [%3, %4]")
            .arg(startX, 1, 'f', 2).arg(endX, 1, 'f', 2)
            .arg(startY, 1, 'f', 2).arg(endY, 1, 'f', 2);
    QLabel *label = new QLabel(this);
    label->setContextMenuPolicy(Qt::CustomContextMenu);
    label->setStyleSheet("border: 1px solid rgba(200,200,200,0.8); background: rgba(200,200,200,0.5);");
    label->setGeometry(geometry);
    label->setText(text);
    label->setToolTip(text);
    label->show();

    // 保存数据，然后通过 property("startY").toDouble() 取出
    label->setProperty("startX", startX);
    label->setProperty("endX",   endX);
    label->setProperty("startY", startY);
    label->setProperty("endY",   endY);

    selections.append(label);

    // Label 上点击右键删除 label
    connect(label, &QLabel::customContextMenuRequested, [label, this] {
        selections.removeAll(label);
        label->deleteLater();
    });

    return label;
}

