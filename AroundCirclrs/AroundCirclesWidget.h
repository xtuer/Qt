#ifndef AROUNDCIRCLESWIDGET_H
#define AROUNDCIRCLESWIDGET_H

#include <QWidget>
#include <QColor>

class AroundCirclesWidgetPrivate;

/**
 * 中心一个大圆，周围围绕 2 圈小圆的 Wiget
 */
class AroundCirclesWidget : public QWidget {
    Q_OBJECT

public:
    explicit AroundCirclesWidget(QWidget *parent = nullptr);
    ~AroundCirclesWidget() override;

    // 设置 id 为传入的 id 的小圆的背景色
    void setCircleColor(int id, const QColor bgcolor);

protected:
    void paintEvent(QPaintEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    AroundCirclesWidgetPrivate *d;
};

#endif // AROUNDCIRCLESWIDGET_H
