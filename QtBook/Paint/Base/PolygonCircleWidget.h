#ifndef POLYGONCIRCLEWIDGET_H
#define POLYGONCIRCLEWIDGET_H

#include <QWidget>

namespace Ui {
class PolygonCircleWidget;
}

/**
 * @brief 使用多边形拟合画圆
 */
class PolygonCircleWidget : public QWidget {
    Q_OBJECT

public:
    explicit PolygonCircleWidget(QWidget *parent = 0);
    ~PolygonCircleWidget();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::PolygonCircleWidget *ui;
};

#endif // POLYGONCIRCLEWIDGET_H
