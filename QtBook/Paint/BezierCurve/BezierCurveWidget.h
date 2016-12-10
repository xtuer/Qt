#ifndef BEZIERCURVEWIDGET_H
#define BEZIERCURVEWIDGET_H

#include <QWidget>
#include <QPointF>
#include <QList>
#include <QPainterPath>

class BezierCurveWidget : public QWidget {
    Q_OBJECT
public:
    explicit BezierCurveWidget(QWidget *parent = 0);
    ~BezierCurveWidget();

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    /**
     * 使用 breakPoints 和 controlPoints 创建贝塞尔曲线
     * 当控制点的坐标变化后都重新生成一次贝塞尔曲线
     */
    QPainterPath createBezierCurve();

    /**
     * 创建显示控制点的圆的 bounding rectangle
     * @param index 控制点在 controlPoints 中的下标
     */
    QRect createControlPointBundingRect(int index);

    /**
     * 为了设置坐标方便，从 (0, 0) 开始设置而不是实际绘制的坐标，对绘制贝塞尔曲线和控制点的
     * 坐标系做了偏移，在计算的时候，坐标也要是相对于新坐标系的坐标才行，不能是原始的坐标，
     * 所以要对其也好做相应的偏移
     * @param point - 例如鼠标按下时在 widget 上的原始坐标
     */
    QPointF translatedPoint(const QPointF &point) const;

private:
    QPainterPath bezierCurve;       // 贝塞尔曲线
    QList<QPointF *> breakPoints;   // 贝塞尔曲线端点的坐标
    QList<QPointF *> controlPoints; // 贝塞尔曲线控制点的坐标

    int pressedControlPointIndex;   // 鼠标按住的控制点在 controlPoints 里的下标
    int controlPointRadius;         // 显示控制点的圆的半径
    int translatedX; // 坐标系 X 轴的偏移量
    int translatedY; // 坐标系 Y 轴的偏移量
    int flags;       // 文本显示的参数
};
#endif // BEZIERCURVEWIDGET_H
