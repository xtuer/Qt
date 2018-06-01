#ifndef SCATTER_H
#define SCATTER_H

#include <QWidget>

/**
 * 布点图上的点，可以拖拽移动、右键删除、tooltip 显示坐标
 */
class Scatter : public QWidget {
    Q_OBJECT
public:
    explicit Scatter(double xRatio, double yRatio, QWidget *parent);
    void setPosition(double xRation, double yRation); // 设置坐标
    void updatePosition(); // 更新坐标

signals:
    void aboutRemove(Scatter *scatter);

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    double xRatio = 0; // 在父 widget 中的横坐标，使用比例
    double yRatio = 0; // 在父 widget 中的纵坐标，使用比例
    QPoint topLeftPositionBeforeMoving; // 移动前左上角的坐标
    QPoint mousePressedPosition;        // 按下鼠标时鼠标的全局坐标
};

#endif // SCATTER_H
