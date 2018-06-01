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

    void updatePosition(); // 更新坐标
    void setPosition(double xRation, double yRation); // 设置坐标
    double getXRatio() const;   // 获取 scatter 在父 widget 中的横坐标，使用比例
    double getYRatio() const;   // 获取 scatter 在父 widget 中的纵坐标，使用比例
    QPoint getPosition() const; // 获取 scatter 的坐标，使用绝对坐标

signals:
    void aboutToRemove(Scatter *scatter); // 即将被删除的信号
    void positionChanged(QPoint pos);     // 位置变化时发射的信号

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

private:
    void createContextMenu(); // 创建右键菜单

    double xRatio = 0; // 在父 widget 中的横坐标，使用比例
    double yRatio = 0; // 在父 widget 中的纵坐标，使用比例
    QPoint topLeftPositionBeforeMoving; // 移动前左上角的坐标
    QPoint mousePressedPosition;        // 按下鼠标时鼠标的全局坐标
};

#endif // SCATTER_H
