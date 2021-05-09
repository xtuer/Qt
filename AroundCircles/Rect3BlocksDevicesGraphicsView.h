#ifndef RECT3BLOCKSDEVICESGRAPHICSVIEW_H
#define RECT3BLOCKSDEVICESGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>

class Rect3BlocksDevicesGraphicsViewPrivate;

/**
 * @brief 有 3 个矩形区域的布点图
 */
class Rect3BlocksDevicesGraphicsView : public QGraphicsView {
public:
    Rect3BlocksDevicesGraphicsView(QWidget *parent = nullptr);
    ~Rect3BlocksDevicesGraphicsView();

private:
    Rect3BlocksDevicesGraphicsViewPrivate *d = nullptr;
};

#endif // RECT3BLOCKSDEVICESGRAPHICSVIEW_H
