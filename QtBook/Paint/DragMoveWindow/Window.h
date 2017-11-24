#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class Window : public QWidget {
    Q_OBJECT

public:
    Window(QWidget *parent = 0);
    ~Window();

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    QPoint windowPositionBeforeMoving; // 移动窗口前窗口左上角的坐标
    QPoint mousePressedPosition;       // 按下鼠标时鼠标的全局坐标
};

#endif // WINDOW_H
