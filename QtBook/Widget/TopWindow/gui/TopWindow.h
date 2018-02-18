#ifndef TOPWINDOW_H
#define TOPWINDOW_H

#include <QWidget>

class NinePatchPainter;

namespace Ui {
class TopWindow;
}

class TopWindow : public QWidget {
    Q_OBJECT

public:
    explicit TopWindow(QWidget *centralWidget);
    ~TopWindow();

    void setTitle(const QString &title);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::TopWindow *ui;
    NinePatchPainter *ninePatchPainter;
    QPoint windowPositionBeforeMoving; // 移动窗口前窗口左上角的坐标
    QPoint mousePressedPosition;       // 按下鼠标时鼠标的全局坐标
};

#endif // TOPWINDOW_H
