#include "SelectableChartView.h"
#include "RecordCalibrationWidget.h"
#include "GridLine.h"

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
#include <QPainter>

SelectableChartView::SelectableChartView(QChart *chart) : QChartView(chart) {
    // TODO: 会选择选区内的点，点多时效率比较低，业务不需要，可以优化, 改为 QRubberBand
    setDragMode(QGraphicsView::RubberBandDrag);
    setRenderHint(QPainter::Antialiasing);
    chart->installEventFilter(this);

    // 灭菌辅助线的 Widget
    sterilizationMarkerWidget = new QWidget(this);
    sterilizationMarkerWidget->setAttribute(Qt::WA_TransparentForMouseEvents);
    sterilizationMarkerWidget->installEventFilter(this);
    sterilizationMarkerWidget->setVisible(false);

    createContextMenu();
    handleLegend();
}

QList<QLabel*> SelectableChartView::getSelections() const {
    return selectionLabels;
}

void SelectableChartView::clearSelections() {
    while (!selectionLabels.isEmpty()) {
        QLabel *label = selectionLabels.at(0);
        selectionLabels.removeAll(label);
        label->deleteLater();
    }
}

void SelectableChartView::clearZoomStatcks() {
    zoomStacks.clear();
}

// 获取水平时间轴的最小值
QDateTime SelectableChartView::getMinDateTime() const {
    QDateTimeAxis *axis = getDateTimeAxis();
    return (NULL != axis) ? axis->min() : QDateTime();
}

// 获取水平时间轴的最大值
QDateTime SelectableChartView::getMaxDateTime() const {
    QDateTimeAxis *axis = getDateTimeAxis();
    return (NULL != axis) ? axis->max() : QDateTime();
}

// 获取左边垂直坐标轴的最小温度
double SelectableChartView::getMinTemperature() const {
    QValueAxis *axis = getTemperatureAxis();
    return (NULL != axis) ? axis->min() : INT_MAX;
}

// 获取左边垂直坐标轴的最高温度
double SelectableChartView::getMaxTemperature() const {
    QValueAxis *axis = getTemperatureAxis();
    return (NULL != axis) ? axis->max() : INT_MIN;
}

// 获取水平的时间坐标轴
QDateTimeAxis *SelectableChartView::getDateTimeAxis() const {
    QList<QAbstractAxis*> axes = chart()->axes(Qt::Horizontal);
    if (axes.size() > 0 && QAbstractAxis::AxisTypeDateTime == axes[0]->type()) {
        return qobject_cast<QDateTimeAxis*>(axes[0]);
    }

    return NULL;
}

// 获取左边垂直坐标轴的温度坐标轴
QValueAxis *SelectableChartView::getTemperatureAxis() const {
    QList<QAbstractAxis*> axes = chart()->axes(Qt::Vertical);

    if (axes.size() > 0 && QAbstractAxis::AxisTypeValue == axes[0]->type()) {
        return qobject_cast<QValueAxis*>(axes[0]);
    }

    return NULL;
}

// 设置灭菌参数
void SelectableChartView::setSterilizationParams(const QDateTime &sterilizeStartTime,
                                                 const QDateTime &sterilizeEndTime,
                                                 double sterilizeTemperature) {
    this->sterilizeStartTime = sterilizeStartTime;
    this->sterilizeEndTime = sterilizeEndTime;
    this->sterilizeTemperature = sterilizeTemperature;
}

// 设置是否显示灭菌的辅助线
void SelectableChartView::setSterilizationMarkersVisible(bool visible) {
    this->sterilizationMarkerWidget->setVisible(visible);
}

bool SelectableChartView::eventFilter(QObject *watched, QEvent *event) {
    // 在 QChart 上按下鼠标时保存按下的坐标
    if (watched == chart() && event->type() == QEvent::GraphicsSceneMousePress) {
        QGraphicsSceneMouseEvent *me = static_cast<QGraphicsSceneMouseEvent*>(event);
        pressedPointAtChart = QPoint(me->pos().x(), me->pos().y());
    } else if (watched == sterilizationMarkerWidget && event->type() == QEvent::Paint) {
        drawSterilizationMarkers();
    }

    return QChartView::eventFilter(watched, event);
}

void SelectableChartView::resizeEvent(QResizeEvent *event) {
    QChartView::resizeEvent(event);

    if (chart()) {
        sterilizationMarkerWidget->setGeometry(chart()->plotArea().toRect());
    }
}

void SelectableChartView::mousePressEvent(QMouseEvent *event) {
    // 鼠标右键点击时不处理
    if (event->buttons() & Qt::RightButton) {
        return;
    }

    pressedPointAtChartView = event->pos();

    if (hLineAction->isChecked()) {
        // 创建水平线
        addGridLine(Qt::Horizontal, event->pos());
    } else if (vLineAction->isChecked()) {
        // 创建垂直线
        addGridLine(Qt::Vertical, event->pos());
    } else {
        QChartView::mousePressEvent(event);
    }
}

// 鼠标送开始根据 rubber band 计算选区的横纵坐标访问和显示选区
void SelectableChartView::mouseReleaseEvent(QMouseEvent *event) {
    if (hLineAction->isChecked() || vLineAction->isChecked()) { return; }

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
            qreal min   = timeAxis->min().toMSecsSinceEpoch();
            qreal max   = timeAxis->max().toMSecsSinceEpoch();
            qreal unit  = (max-min) / plotRect.width();
            qreal start = min   + unit * selectRect.x();
            qreal end   = start + unit * selectRect.width();

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
    selectionLabels.append(label);

    QRect plotRect = chart()->plotArea().toRect();
    CalibrationRange calibrationRange;

    for (AxisRange range : ranges) {
        bool top    = (range.axis->alignment() & Qt::AlignTop)    != 0;
        bool bottom = (range.axis->alignment() & Qt::AlignBottom) != 0;
        bool left   = (range.axis->alignment() & Qt::AlignLeft)   != 0;
        bool right  = (range.axis->alignment() & Qt::AlignRight)  != 0;
        bool isDtx  = QAbstractAxis::AxisTypeDateTime == range.axis->type(); // 时间坐标轴

        // 上下的都是水平坐标轴，左右的都是纵坐标轴
        if (xAction->isChecked() && (top || bottom)) {
            // 只取横坐标轴的
            label->setProperty("type", "axisX");
            text += QString("%1: %2, %3\n").arg(range.axis->objectName())
                    .arg(isDtx ? range.min.toDateTime().toString("\nyyyy-MM-dd HH:mm:ss") : QString::number(range.min.toReal()))
                    .arg(isDtx ? range.max.toDateTime().toString("\nyyyy-MM-dd HH:mm:ss") : QString::number(range.max.toReal()));

            geometry.setY(plotRect.y());
            geometry.setHeight(plotRect.height());

            if (isDtx) {
                calibrationRange.chart = this->chart();
                calibrationRange.minTime = range.min.toDateTime();
                calibrationRange.maxTime = range.max.toDateTime();
                calibrationRanges.insert(label, calibrationRange);
            }
        } else if (yAction->isChecked() && (left || right)) {
            // 只取纵坐标轴的
            label->setProperty("type", "axisY");
            text += QString("%1: %2, %3\n").arg(range.axis->objectName())
                    .arg(isDtx ? range.min.toDateTime().toString("\nyyyy-MM-dd HH:mm:ss") : QString::number(range.min.toReal()))
                    .arg(isDtx ? range.max.toDateTime().toString("\nyyyy-MM-dd HH:mm:ss") : QString::number(range.max.toReal()));

            geometry.setX(plotRect.x());
            geometry.setWidth(plotRect.width());
        } else if (xyAction->isChecked()) {
            // 横纵坐标轴的都取
            label->setProperty("type", "axisXY");
            text += QString("%1: %2, %3\n").arg(range.axis->objectName())
                    .arg(isDtx ? range.min.toDateTime().toString("\nyyyy-MM-dd HH:mm:ss") : QString::number(range.min.toReal()))
                    .arg(isDtx ? range.max.toDateTime().toString("\nyyyy-MM-dd HH:mm:ss") : QString::number(range.max.toReal()));
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
        selectionLabels.removeAll(label);
        calibrationRanges.remove(label);
        label->deleteLater();
    });
}

// 创建右键菜单
void SelectableChartView::createContextMenu() {
    xAction  = new QAction("只有横坐标", this);
    yAction  = new QAction("只有纵坐标", this);
    xyAction = new QAction("横纵坐标都有", this);
    zoomInAction    = new QAction("图像放大", this);
    zoomOutAction   = new QAction("图像缩小", this);
    zoomResetAction = new QAction("图像复位", this);
    calibrationAction = new QAction("记录校准器", this);
    hLineAction = new QAction("水平线", this);
    vLineAction = new QAction("垂直线", this);

    xAction->setCheckable(true);
    yAction->setCheckable(true);
    xyAction->setCheckable(true);
    xyAction->setChecked(true);
    zoomInAction->setCheckable(true);
    hLineAction->setCheckable(true);
    vLineAction->setCheckable(true);

    QActionGroup *group = new QActionGroup( this );
    group->addAction(xAction);
    group->addAction(yAction);
    group->addAction(xyAction);
    group->addAction(zoomInAction);
    group->addAction(hLineAction);
    group->addAction(vLineAction);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested, [this](const QPoint &pos) {
        QMenu menu;
        menu.addAction(xAction);
        menu.addAction(yAction);
        menu.addAction(xyAction);
        menu.addSeparator();
        menu.addAction(hLineAction);
        menu.addAction(vLineAction);
        menu.addSeparator();
        menu.addAction(zoomInAction);
        menu.addAction(zoomOutAction);
        menu.addAction(zoomResetAction);
        menu.addSeparator();
        menu.addAction(calibrationAction);

        zoomOutAction->setEnabled(!zoomStacks.isEmpty());
        zoomResetAction->setEnabled(!zoomStacks.isEmpty());
        calibrationAction->setEnabled(calibrationRanges.size() > 0);

        menu.exec(mapToGlobal(pos));
    });

    connect(zoomOutAction, &QAction::triggered, [this] {
        // 栈里有放大的数据时才恢复上一次的缩放
        if (!zoomStacks.isEmpty()) {
            clearSelections();
            setAxisRanges(zoomStacks.pop());
        }
    });

    connect(zoomResetAction, &QAction::triggered, [this] {
        if (!zoomStacks.isEmpty()) {
            clearSelections();
            setAxisRanges(zoomStacks.first());
            zoomStacks.clear();
        }
    });

    // 弹出记录器校准窗口
    connect(calibrationAction, &QAction::triggered, [this] {
        if (calibrationRanges.size() == 0) { return; }
        RecordCalibrationWidget *cw = new RecordCalibrationWidget(calibrationRanges.values());
        cw->show();
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

// 在图表上增加网格线
void SelectableChartView::addGridLine(Qt::Orientation orientation, QPoint pos) {
    QRectF rect = chart()->plotArea();

    if (Qt::Horizontal == orientation) {
        // 创建水平线
        GridLine *hLine = new GridLine(Qt::Horizontal, chart(), this);
        hLine->setGeometry(rect.x(), pos.y(), rect.width(), pos.y());
        hLine->show();
    } else if (Qt::Vertical == orientation) {
        // 创建垂直线
        GridLine *vLine = new GridLine(Qt::Vertical, chart(), this);
        vLine->setGeometry(pos.x(), rect.y(), pos.x(), rect.height());
        vLine->show();
    }
}

// 绘制灭菌辅助线
void SelectableChartView::drawSterilizationMarkers() {
    QPainter painter(sterilizationMarkerWidget);
    painter.setPen(QPen(Qt::darkGray, 2, Qt::DashLine));

    // 加粗字体
    QFont font = painter.font();
    font.setBold(true);
    painter.setFont(font);

    int w = sterilizationMarkerWidget->width();
    int h = sterilizationMarkerWidget->height();

    double tMin = getMinTemperature();
    double tMax = getMaxTemperature();

    // 绘制温度辅助线
    int ty = (int)(h - (sterilizeTemperature-tMin) / (tMax - tMin) * h);
    painter.drawLine(0, ty, width(), ty);
    painter.drawText(10, ty+15, "灭菌设定温度");


    qint64 dMin = getMinDateTime().toMSecsSinceEpoch();
    qint64 dMax = getMaxDateTime().toMSecsSinceEpoch();
    qint64 s = sterilizeStartTime.toMSecsSinceEpoch();
    qint64 e = sterilizeEndTime.toMSecsSinceEpoch();

    int sx = (int)((double)(s-dMin)/(dMax-dMin) * w); // 灭菌开始的横坐标
    int ex = (int)((double)(e-dMin)/(dMax-dMin) * w); // 灭菌结束的横坐标

    // 绘制灭菌开始的辅助线
    painter.save();
    painter.drawLine(sx, 0, sx, h);
    painter.translate(sx, 0);
    painter.rotate(90);
    painter.drawText(5, -5, getMinDateTime().toString("灭菌开始时间 yyyy-MM-dd HH:mm:ss"));
    painter.restore();

    // 绘制灭菌结束的辅助线
    painter.save();
    painter.drawLine(ex, 0, ex, h);
    painter.translate(ex, 0);
    painter.rotate(90);
    painter.drawText(5, -5, getMaxDateTime().toString("灭菌结束时间 yyyy-MM-dd HH:mm:ss"));
    painter.restore();
}
