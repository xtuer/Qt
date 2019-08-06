#ifndef PIXMAPDEVICESGRAPHICSVIEW_H
#define PIXMAPDEVICESGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>

class PixmapDevicesGraphicsViewPrivate;

class PixmapDevicesGraphicsView : public QGraphicsView {
public:
    PixmapDevicesGraphicsView(QWidget *parent = nullptr);
    ~PixmapDevicesGraphicsView() override;

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    PixmapDevicesGraphicsViewPrivate *d;
};

#endif // PIXMAPDEVICESGRAPHICSVIEW_H
