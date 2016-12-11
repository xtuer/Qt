#ifndef SMOOTHCURVEGENERATOR_H
#define SMOOTHCURVEGENERATOR_H

#include <QList>
#include <QPointF>
#include <QPainterPath>

class SmoothCurveGenerator {
public:
    /**
     * 传入曲线上的点的 list，创建平滑曲线
     *
     * @param points - 曲线上的点
     * @return - 返回使用给定的点创建的 QPainterPath 表示的平滑曲线
     */
    static QPainterPath generateSmoothCurve(const QList<QPointF> &points);
};

#endif // SMOOTHCURVEGENERATOR_H
