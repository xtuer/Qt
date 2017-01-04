#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

class IntroductionWidget;

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget {
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

protected:
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::MainWidget *ui;
    IntroductionWidget *introductionWidget;

    QPoint mousePressedPosition; // 鼠标按下时的全局坐标坐标
    QPoint windowPositionAsMousePressed;
    QPixmap background;
};

#endif // MAINWIDGET_H
