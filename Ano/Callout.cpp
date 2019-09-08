#include "Callout.h"

#include <QPainter>
#include <QFontMetrics>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <QChart>
#include <QXYSeries>

Callout::Callout(QChart *chart): QGraphicsObject(chart), m_chart(chart) {
}

QRectF Callout::boundingRect() const {
    if (nullptr != m_series) {
        QPointF anchor = mapFromParent(m_chart->mapToPosition(m_anchor, m_series));
        QRectF rect;
        rect.setLeft(qMin(m_rect.left(), anchor.x()));
        rect.setRight(qMax(m_rect.right(), anchor.x()));
        rect.setTop(qMin(m_rect.top(), anchor.y()));
        rect.setBottom(qMax(m_rect.bottom(), anchor.y()));

        return rect.adjusted(-15, -15, 15, 15);
    } else {
        return QRectF();
    }
}

// 碰撞检测使用，也是我们看到的 callout 外形
QPainterPath Callout::shape() const {
    Callout *self = const_cast<Callout *>(this);
    QPointF anchor = mapFromParent(m_chart->mapToPosition(m_anchor, m_series));
    QPainterPath path;

    self->m_rect.moveTo(anchor.x() + 10, anchor.y() - m_rect.height() - 5);
    path.addRoundedRect(m_rect, 5, 5);

    path.moveTo(m_rect.x(), m_rect.y() + m_rect.height() - 15);
    path.lineTo(anchor);
    path.lineTo(m_rect.x(), m_rect.y() + m_rect.height() - 5);
    path = path.simplified();

    return path;
}

void Callout::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setBrush(m_color);
    painter->drawPath(shape());
    painter->drawText(m_rect.adjusted(5, 5, -5, -5), m_text);
}

void Callout::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *) {
    emit doubleClicked(this);
}

void Callout::setText(const QString &text) {
    m_text = text;
    QFontMetrics metrics(m_font);
    QRectF textRect = metrics.boundingRect(QRect(0, 0, 150, 150), Qt::AlignLeft, m_text);
    textRect.translate(5, 5);
    prepareGeometryChange();
    m_rect = textRect.adjusted(-5, -5, 5, 5);
}

void Callout::setAnchor(QPointF point, QXYSeries *series) {
    m_series = series;
    m_anchor = point;
    m_coordinate = point;
    m_color = series->color();
}

void Callout::updateGeometry() {
    prepareGeometryChange();
    setPos(m_chart->mapToPosition(m_anchor, m_series) + QPoint(10, -50));
}
