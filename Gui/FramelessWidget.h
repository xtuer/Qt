#ifndef FRAMELESSWIDGET_H
#define FRAMELESSWIDGET_H

#include <QWidget>

class FramelessWidget : public QWidget {
    Q_OBJECT
public:
    explicit FramelessWidget(QWidget *centralWidget, QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;

private:
    QPoint pressedMousePosition;      // 鼠标按下时的坐标
    QPoint topLeftPositionBeforeDrag; // 鼠标按小时窗口左上角的坐标
};

#endif // FRAMELESSWIDGET_H
