#ifndef AROUNDCIRCLESGRAPHICSVIEW_H
#define AROUNDCIRCLESGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>

class AroundCirclesGraphicsViewPrivate;

class AroundCirclesGraphicsView : public QGraphicsView {
public:
    AroundCirclesGraphicsView(QWidget *parent = nullptr);
    ~AroundCirclesGraphicsView();

    /**
     * 设置名字为传入的 name 的圆的背景色，圆的名字在拖放时自动设置
     *
     * @param name    圆的名字
     * @param bgcolor 背景色
     */
    void setCircleBgcolor(const QString &name, const QString &bgcolor);

private:
    AroundCirclesGraphicsViewPrivate *d = nullptr;
};

#endif // AROUNDCIRCLESGRAPHICSVIEW_H
