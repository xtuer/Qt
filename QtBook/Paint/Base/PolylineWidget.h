#ifndef POLYLINEWIDGET_H
#define POLYLINEWIDGET_H

#include <QWidget>

namespace Ui {
class PolylineWidget;
}

/**
 * @brief Polyline
 */
class PolylineWidget : public QWidget {
    Q_OBJECT

public:
    explicit PolylineWidget(QWidget *parent = 0);
    ~PolylineWidget();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::PolylineWidget *ui;
};

#endif // POLYLINEWIDGET_H
