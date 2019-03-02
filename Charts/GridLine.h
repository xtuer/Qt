#ifndef GRIDLINE_H
#define GRIDLINE_H

#include <QWidget>

/**
 * 网格线: 垂直的和水平的
 */
class GridLine : public QWidget {
    Q_OBJECT
public:
    explicit GridLine(Qt::Orientation orientation, QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    QPoint topLeftBeforeMoving; // 移动窗口前窗口左上角的坐标
    QPoint mousePressedPosition;       // 按下鼠标时鼠标的全局坐标
    Qt::Orientation orientation; // Qt::Horizontal or Qt::Vertical
};

#endif // GRIDLINE_H
