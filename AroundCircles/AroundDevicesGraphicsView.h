#ifndef AROUNDDEVICESGRAPHICSVIEW_H
#define AROUNDDEVICESGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>

class AroundDevicesGraphicsViewPrivate;

/**
 * 此 View 的中间是一个大圆 (DialPlate)，围绕大圆放置圆形设备 (CircleDevice)
 */
class AroundDevicesGraphicsView : public QGraphicsView {
public:
    AroundDevicesGraphicsView(QWidget *parent = nullptr);
    ~AroundDevicesGraphicsView();

    /**
     * 设置名字为传入的 name 的圆的背景色，圆的名字在拖放时自动设置
     *
     * @param name    圆的名字
     * @param bgcolor 背景色
     */
    void setCircleDeviceBgcolor(const QString &name, const QString &bgcolor);

private:
    AroundDevicesGraphicsViewPrivate *d = nullptr;
};

#endif // AROUNDDEVICESGRAPHICSVIEW_H
