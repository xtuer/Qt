#ifndef CONICALGRADIENTWIDGET_H
#define CONICALGRADIENTWIDGET_H

#include <QWidget>

namespace Ui {
class ConicalGradientWidget;
}

/**
 * @brief 圆锥渐变
 */
class ConicalGradientWidget : public QWidget {
    Q_OBJECT

public:
    explicit ConicalGradientWidget(QWidget *parent = 0);
    ~ConicalGradientWidget();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::ConicalGradientWidget *ui;
};

#endif // CONICALGRADIENTWIDGET_H
