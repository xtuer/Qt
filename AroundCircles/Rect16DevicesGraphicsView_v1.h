#ifndef RECT16DEVICESGRAPHICSVIEW_H_V1
#define RECT16DEVICESGRAPHICSVIEW_H_V1

#include <QGraphicsView>
#include <QGraphicsScene>

class Rect16DevicesGraphicsViewPrivate_v1;

/**
 * @brief 16 个布点占位
 */
class Rect16DevicesGraphicsView_v1 : public QGraphicsView {
public:
    Rect16DevicesGraphicsView_v1(QWidget *parent = nullptr);
    ~Rect16DevicesGraphicsView_v1();

private:
    Rect16DevicesGraphicsViewPrivate_v1 *d = nullptr;
};

#endif // RECT16DEVICESGRAPHICSVIEW_H_V1
