#ifndef WIDGET_H
#define WIDGET_H

#include <QGraphicsView>

class Widget : public QGraphicsView {
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
};

#endif // WIDGET_H
