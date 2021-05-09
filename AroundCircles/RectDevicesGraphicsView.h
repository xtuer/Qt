#ifndef RECTDEVICESGRAPHICSVIEW_H
#define RECTDEVICESGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>

class RectDevicesGraphicsViewPrivate;

/**
 * @brief 指定行列，生成布点占位
 */
class RectDevicesGraphicsView : public QGraphicsView {
public:
    RectDevicesGraphicsView(int horizontalCount, int verticalCount, QWidget *parent = nullptr);
    ~RectDevicesGraphicsView();

private:
    RectDevicesGraphicsViewPrivate *d = nullptr;
};

#endif // RECTDEVICESGRAPHICSVIEW_H
