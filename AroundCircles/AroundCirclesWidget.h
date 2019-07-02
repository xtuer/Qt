#ifndef AROUNDCIRCLESWIDGET_H
#define AROUNDCIRCLESWIDGET_H

#include <QWidget>
#include <QColor>

class AroundCirclesWidgetPrivate;

/**
 * 中心一个大圆，周围围绕 2 圈小圆的 Wiget，
 *
 * 支持拖放到小圆上时设置它的背景色:
 *     拖拽的 type 为 DnD-DEVICE-CIRCLE
 *     内容为字符串格式 deviceId,color，如 "device-1,#rrggbb"
 */
class AroundCirclesWidget : public QWidget {
    Q_OBJECT

public:
    explicit AroundCirclesWidget(QWidget *parent = nullptr);
    ~AroundCirclesWidget() override;

protected:
    void paintEvent(QPaintEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    AroundCirclesWidgetPrivate *d;
};

#endif // AROUNDCIRCLESWIDGET_H
