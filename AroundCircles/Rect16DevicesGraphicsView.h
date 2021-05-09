#ifndef RECT16DEVICESGRAPHICSVIEW_H
#define RECT16DEVICESGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>

class Rect16DevicesGraphicsViewPrivate;

/**
 * @brief 16 个布点占位
 */
class Rect16DevicesGraphicsView : public QGraphicsView {
public:
    Rect16DevicesGraphicsView(QWidget *parent = nullptr);
    ~Rect16DevicesGraphicsView();

private:
    Rect16DevicesGraphicsViewPrivate *d = nullptr;
};

#endif // RECT16DEVICESGRAPHICSVIEW_H
