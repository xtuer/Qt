#ifndef REALTIMEWIDGET_H
#define REALTIMEWIDGET_H

#include <QWidget>
#include <QList>
#include <QPainterPath>

namespace Ui {
class RealTimeCurveWidget;
}

class RealTimeWidget : public QWidget {
    Q_OBJECT

public:
    explicit RealTimeWidget(QWidget *parent = 0);
    ~RealTimeWidget();

protected:
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *e) Q_DECL_OVERRIDE;

private:
    /**
     * 接收到数据源发送来的数据，数据源可以下位机，采集卡，传感器等。
     */
    void dataReceived(int value);

    /**
     * 生成数据的曲线
     */
    void createCurve();

    /**
     * 根据数据生成要表示数据的曲线上点的坐标
     * @param w - 数据曲线所在矩形的宽
     * @param h - 数据曲线所在矩形的高
     * @return - 返回曲线上点的坐标的 list
     */
    QList<QPointF> createCurveKnots() const;

    Ui::RealTimeCurveWidget *ui;
    int timerId;
    int maxSize;  // data 最多存储 maxSize 个元素
    int maxValue; // 业务数据的最大值
    QList<double> data; // 存储业务数据的 list

    int w; // 数据的曲线所在矩形的宽
    int h; // 数据的曲线所在矩形的高
    QPainterPath curve; // 数据的曲线
};

#endif // REALTIMEWIDGET_H
