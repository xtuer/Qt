#ifndef OSUI_H
#define OSUI_H

#include <QWidget>
#include <QList>

class Widget;

/**
 * 操作系统的界面，在它的上面放置各种控件。
 */
class OSUi : public QWidget {
    Q_OBJECT

public:
    explicit OSUi(QWidget *parent = 0);
    ~OSUi();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    QList<Widget*> widgets; // 界面上所有的控件的集合
};

#endif // OSUI_H
