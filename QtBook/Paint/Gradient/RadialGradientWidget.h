#ifndef RADIALGRADIENTWIDGET_H
#define RADIALGRADIENTWIDGET_H

#include <QWidget>

namespace Ui {
class RadialGradientWidget;
}

/**
 * @brief 径向渐变
 */
class RadialGradientWidget : public QWidget {
    Q_OBJECT

public:
    explicit RadialGradientWidget(QWidget *parent = 0);
    ~RadialGradientWidget();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::RadialGradientWidget *ui;
};

#endif // RADIALGRADIENTWIDGET_H
