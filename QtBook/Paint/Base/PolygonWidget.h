#ifndef POLYGONWIDGET_H
#define POLYGONWIDGET_H

#include <QWidget>

namespace Ui {
class PolygonWidget;
}

/**
 * @brief 画多边形
 */
class PolygonWidget : public QWidget {
    Q_OBJECT

public:
    explicit PolygonWidget(QWidget *parent = 0);
    ~PolygonWidget();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::PolygonWidget *ui;
};

#endif // POLYGONWIDGET_H
