#include "SelectableChartView.h"

#include <QDebug>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QValueAxis>
#include <QDateTimeAxis>
#include <QLabel>
#include <QAction>
#include <QMenu>
#include <QChart>
#include <QActionGroup>
#include <QLegendMarker>
#include <QDateTime>

SelectableChartView::SelectableChartView(QChart *chart) : QChartView(chart) {
    // TODO: 会选择选区内的点，点多时效率比较低，业务不需要，可以优化, 改为 QRubberBand
    setDragMode(QGraphicsView::RubberBandDrag);
    setRenderHint(QPainter::Antialiasing);
    chart->installEventFilter(this);

    createContextMenu();
    handleLegend();
}

QList<QLabel*> SelectableChartView::getSelections() const {
    return selections;
}

void SelectableChartView::clearSelections() {
    while (!selections.isEmpty()) {
        QLabel *label = selections.at(0);
        selections.removeAll(label);
        label->deleteLater();
    }
}

bool SelectableChartView::eventFilter(QObject *watched, QEvent *event) {
    // 在 QChart 上按下鼠标时保存按下的坐标
    if (watched == chart() && event->type() == QEvent::GraphicsSceneMousePress) {
        QGraphicsSceneMouseEvent *me = static_cast<QGraphicsSceneMouseEvent*>(event);
        pressedPointAtChart = QPoint(me->pos().x(), me->pos().y());
    }

    return QChartView::eventFilter(watched, event);
}

void SelectableChartView::mousePressEvent(QMouseEvent *event) {
    pressedPointAtChartView = event->pos();
    QChartView::mousePressEvent(event);
}

// 鼠标送开始根据 rubber band 计算选区的横纵坐标访问和显示选区
void SelectableChartView::mouseReleaseEvent(QMouseEvent *event) {
    // 处理从左下向右上选取的情况，校对坐标
    if (event->pos().x() < pressedPointAtChartView.x()) {
        pressedPointAtChart.setX(pressedPointAtChart.x() - rubberBandRect().width());
    }

    if (event->pos().y() < pressedPointAtChartView.y()) {
        pressedPointAtChart.setY(pressedPointAtChart.y() - rubberBandRect().height());
    }

    // 选区大于 20x20 时才处理
    if (rubberBandRect().width() > 20 && rubberBandRect().height() > 20) {
        createSelection();
    }

    // 让父类处理事件
    QChartView::mouseReleaseEvent(event);
}

// 创建选区
void SelectableChartView::createSelection() {
    // QChartView 上的选区映射到 QChart 绘制曲线的矩形上, selectRect 的左下角和坐标轴上的原点重合
    QRectF plotRect = chart()->plotArea();
    QRectF selectRect(pressedPointAtChart - plotRect.topLeft(), QSize(rubberBandRect().size()));

    QList<QAbstractAxis*> axes = chart()->axes(); // 所有的坐标轴
    QList<AxisRange> currentRanges;  // 当前坐标的范围
    QList<AxisRange> selectedRanges; // 选中区的坐标范围

    // 遍历所有的坐标轴，计算坐标范围
    for (QAbstractAxis *axis : axes) {
        // 坐标不显示时也不显示它的坐标范围
        if (!axis->isVisible()) {
            continue;
        }

        if (QAbstractAxis::AxisTypeValue == axis->type()) {
            // 计算数值轴的范围
            QValueAxis *valueAxis = qobject_cast<QValueAxis*>(axis);
            qreal min   = valueAxis->min();
            qreal max   = valueAxis->max();
            qreal unit  = (max-min) / plotRect.height();
            qreal start = min   + unit * (plotRect.height() - selectRect.y() - selectRect.height());
            qreal end   = start + unit * selectRect.height();

            currentRanges.append({valueAxis, min, max});
            selectedRanges.append({valueAxis, start, end});
        } else if (QAbstractAxis::AxisTypeDateTime == axis->type()) {
            // 计算时间轴的范围
            QDateTimeAxis *timeAxis = qobject_cast<QDateTimeAxis*>(axis);
            qint64 min   = timeAxis->min().toMSecsSinceEpoch();
            qint64 max   = timeAxis->max().toMSecsSinceEpoch();
            qint64 unit  = (max-min) / plotRect.width();
            qint64 start = min   + unit * selectRect.x();
            qint64 end   = start + unit * selectRect.width();

            currentRanges.append({timeAxis, timeAxis->min(), timeAxis->max()});
            selectedRanges.append({timeAxis, QDateTime::fromMSecsSinceEpoch(start), QDateTime::fromMSecsSinceEpoch(end)});
        }
    }

    // 选中放大时进行放大图标
    if (zoomInAction->isChecked()) {
        // 1. 清楚选区
        // 2. 保存当前的坐标访问
        // 3. 设置选区进行放大
        clearSelections();
        zoomStacks.push(currentRanges);
        setAxisRanges(selectedRanges);
    } else {
        createSelectionLabel(selectedRanges, rubberBandRect());
    }
}

void SelectableChartView::setAxisRanges(QList<AxisRange> ranges) {
    for (AxisRange range : ranges) {
        if (QAbstractAxis::AxisTypeValue == range.axis->type()) {
            QValueAxis *valueAxis = qobject_cast<QValueAxis*>(range.axis);
            valueAxis->setRange(range.min.toReal(), range.max.toReal());
        } else if (QAbstractAxis::AxisTypeDateTime == range.axis->type()) {
            QDateTimeAxis *timeAxis = qobject_cast<QDateTimeAxis*>(range.axis);
            timeAxis->setRange(range.min.toDateTime(), range.max.toDateTime());
        }
    }
}

void SelectableChartView::createSelectionLabel(QList<AxisRange> ranges, QRect geometry) {
    QString text;
    QLabel *label = new QLabel(this);
    selections.append(label);

    for (AxisRange range : ranges) {
        bool top    = (range.axis->alignment() & Qt::AlignTop)    != 0;
        bool bottom = (range.axis->alignment() & Qt::AlignBottom) != 0;
        bool left   = (range.axis->alignment() & Qt::AlignLeft)   != 0;
        bool right  = (range.axis->alignment() & Qt::AlignRight)  != 0;
        bool isDtx = QAbstractAxis::AxisTypeDateTime == range.axis->type(); // 时间坐标轴
        QString temp = QString("%1: %2, %3\n").arg(range.axis->objectName())
                .arg(isDtx ? range.min.toDateTime().toString("yyyy-MM-dd HH:ss:mm") : QString::number(range.min.toReal()))
                .arg(isDtx ? range.max.toDateTime().toString("yyyy-MM-dd HH:ss:mm") : QString::number(range.max.toReal()));

        // 上下的都是水平坐标轴，左右的都是纵坐标轴
        if (xAction->isChecked() && (top || bottom)) {
            // 只取横坐标轴的
            label->setProperty("type", "axisX");
            text += temp;
        } else if (yAction->isChecked() && (left || right)) {
            // 只取纵坐标轴的
            label->setProperty("type", "axisY");
            text += temp;
        } else if (xyAction->isChecked()) {
            // 横纵坐标轴的都取
            label->setProperty("type", "axisXY");
            text += temp;
        }
    }

    this->setStyleSheet("QLabel[type='axisX']  { border: 1px solid rgba(000,200,200,0.8); background: rgba(000,200,200,0.5); }"
                        "QLabel[type='axisY']  { border: 1px solid rgba(200,000,200,0.8); background: rgba(200,000,200,0.5); }"
                        "QLabel[type='axisXY'] { border: 1px solid rgba(200,200,000,0.8); background: rgba(200,200,000,0.5); }");

    label->setGeometry(geometry);
    label->setText(text); // 获取 label 的 text，解析得到选择的范围
    label->setToolTip(text);
    label->show();

    // Label 上点击右键删除 label
    label->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(label, &QLabel::customContextMenuRequested, [label, this] {
        selections.removeAll(label);
        label->deleteLater();
    });
}

// 创建右键菜单
void SelectableChartView::createContextMenu() {
    xAction  = new QAction("只有横坐标", this);
    yAction  = new QAction("只有纵坐标", this);
    xyAction = new QAction("横纵坐标都有", this);
    zoomInAction    = new QAction("放大", this);
    zoomResetAction = new QAction("缩小", this);

    xAction->setCheckable(true);
    yAction->setCheckable(true);
    xyAction->setCheckable(true);
    xyAction->setChecked(true);
    zoomInAction->setCheckable(true);

    QActionGroup *group = new QActionGroup( this );
    group->addAction(xAction);
    group->addAction(yAction);
    group->addAction(xyAction);
    group->addAction(zoomInAction);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested, [this](const QPoint &pos) {
        QMenu menu;
        menu.addAction(xAction);
        menu.addAction(yAction);
        menu.addAction(xyAction);
        menu.addSeparator();
        menu.addAction(zoomInAction);
        menu.addAction(zoomResetAction);
        zoomResetAction->setEnabled(!zoomStacks.isEmpty());

        menu.exec(mapToGlobal(pos));
    });

    connect(zoomResetAction, &QAction::triggered, [this] {
        // 栈里有放大的数据时才恢复上一次的缩放
        if (!zoomStacks.isEmpty()) {
            clearSelections();
            setAxisRanges(zoomStacks.pop());
        }
    });
}

void SelectableChartView::handleLegend() {
    SelectableChartView::handleLegend(chart());
}

void SelectableChartView::handleLegend(QChart *chart) {
    // 点击 legend 隐藏和显示曲线，曲线不可见时，半透明它的 legend
    const auto markers = chart->legend()->markers();
    for (QLegendMarker *marker : markers) {
        marker->disconnect();
        connect(marker, &QLegendMarker::clicked, [=] {
            marker->series()->setVisible(!marker->series()->isVisible());
            marker->setVisible(true);

            // 曲线不可见时，半透明它的 legend
            qreal alpha = marker->series()->isVisible() ? 1 : 0.2;
            QColor color;
            QBrush brush;
            QPen pen;

            // 画刷
            brush = marker->labelBrush();
            color = brush.color();
            color.setAlphaF(alpha);
            brush.setColor(color);
            marker->setLabelBrush(brush);

            brush = marker->brush();
            color = brush.color();
            color.setAlphaF(alpha);
            brush.setColor(color);
            marker->setBrush(brush);

            // 画笔
            pen   = marker->pen();
            color = pen.color();
            color.setAlphaF(alpha);
            pen.setColor(color);
            marker->setPen(pen);
        });
    }
}
