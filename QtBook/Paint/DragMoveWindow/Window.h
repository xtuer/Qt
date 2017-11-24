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
    QPoint windowPositionBeforeMoving;
    QPoint mousePressedPosition;
};

#endif // WINDOW_H
