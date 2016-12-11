#ifndef LINEARGRADIENTWIDGET_H
#define LINEARGRADIENTWIDGET_H

#include <QWidget>

namespace Ui {
class LinearGradientWidget;
}

/**
 * @brief 线性渐变
 */
class LinearGradientWidget : public QWidget {
    Q_OBJECT

public:
    explicit LinearGradientWidget(QWidget *parent = 0);
    ~LinearGradientWidget();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::LinearGradientWidget *ui;
};

#endif // LINEARGRADIENTWIDGET_H
