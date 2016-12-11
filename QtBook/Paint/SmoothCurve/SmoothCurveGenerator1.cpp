#include "SmoothCurveGenerator1.h"

QPainterPath SmoothCurveGenerator1::generateSmoothCurve(const QList<QPointF> &points) {
    if (points.size() == 0) {
        return QPainterPath();
    }

    QPainterPath path(points[0]);

    for (int i = 0; i < points.size() - 1; ++i) {
        // 控制点的 x 坐标为 sp 与 ep 的 x 坐标和的一半
        // 第一个控制点 c1 的 y 坐标为起始点 sp 的 y 坐标
        // 第二个控制点 c2 的 y 坐标为结束点 ep 的 y 坐标
        QPointF sp = points[i];
        QPointF ep = points[i+1];
        QPointF c1 = QPointF((sp.x() + ep.x()) / 2, sp.y());
        QPointF c2 = QPointF((sp.x() + ep.x()) / 2, ep.y());
        path.cubicTo(c1, c2, ep);
    }

    return path;
}
