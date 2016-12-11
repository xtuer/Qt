#ifndef SMOOTHCURVEWIDGET_H
#define SMOOTHCURVEWIDGET_H

#include <QWidget>
#include <QList>
#include <QPointF>
#include <QPainterPath>

namespace Ui {
class SmoothCurveWidget;
}

class SmoothCurveWidget : public QWidget {
    Q_OBJECT

public:
    explicit SmoothCurveWidget(QWidget *parent = 0);
    ~SmoothCurveWidget();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private slots:
    void generateCurves(); // 生成平滑曲线和非平滑曲线

private:
    Ui::SmoothCurveWidget *ui;
    QList<QPointF> knots;        // 曲线上的点
    QPainterPath smoothCurve1;   // 平滑曲线
    QPainterPath smoothCurve2;   // 平滑曲线
    QPainterPath nonSmoothCurve; // 直接连接点的非平滑曲线
};

#endif // SMOOTHCURVEWIDGET_H
