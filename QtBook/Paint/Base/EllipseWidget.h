#ifndef ELLIPSEWIDGET_H
#define ELLIPSEWIDGET_H

#include <QWidget>

namespace Ui {
class EllipseWidget;
}

/**
 * @brief 画圆和椭圆
 */
class EllipseWidget : public QWidget {
    Q_OBJECT

public:
    explicit EllipseWidget(QWidget *parent = 0);
    ~EllipseWidget();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::EllipseWidget *ui;
};

#endif // ELLIPSEWIDGET_H
