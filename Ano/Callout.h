#ifndef CALLOUT_H
#define CALLOUT_H

#include <QChartGlobal>
#include <QGraphicsItem>
#include <QGraphicsObject>
#include <QFont>

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChart;
class QXYSeries;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class Callout : public QGraphicsObject {
    Q_OBJECT
public:
    Callout(QChart *parent);

    // 获取 callout 的 x, y 坐标，以及对应的 series
    double x() { return m_coordinate.x(); }
    double y() { return m_coordinate.y(); }
    QXYSeries* series() { return m_series; }

    void setText(const QString &text);
    void setAnchor(QPointF point, QXYSeries *series);
    void updateGeometry();

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget) override;

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    // 双击时的信号，不知道为啥 sender() 获取到的是 NULL，所以传入进去
    void doubleClicked(Callout *sender);

private:
    QString m_text;
    QRectF  m_rect;
    QPointF m_anchor;
    QFont   m_font;
    QChart *m_chart;
    QColor  m_color;
    QXYSeries *m_series = nullptr;

    QPointF m_coordinate; // Callout 的坐标
};

#endif // CALLOUT_H
