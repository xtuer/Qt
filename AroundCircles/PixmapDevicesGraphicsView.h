#ifndef PIXMAPDEVICESGRAPHICSVIEW_H
#define PIXMAPDEVICESGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>

class PixmapDevicesGraphicsViewPrivate;

/**
 * 在图片上布点设备的 view，双击 view 选择背景图，拖放设备到 view 上创建设备的 item。
 */
class PixmapDevicesGraphicsView : public QGraphicsView {
public:
    PixmapDevicesGraphicsView(QWidget *parent = nullptr);
    ~PixmapDevicesGraphicsView() override;

protected:
    void resizeEvent(QResizeEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    PixmapDevicesGraphicsViewPrivate *d;
};

#endif // PIXMAPDEVICESGRAPHICSVIEW_H
