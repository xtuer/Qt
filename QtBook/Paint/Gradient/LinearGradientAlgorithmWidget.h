#ifndef LINEARGRADIENTALGORITHMWIDGET_H
#define LINEARGRADIENTALGORITHMWIDGET_H

#include <QWidget>

namespace Ui {
class LinearGradientAlgorithmWidget;
}

/**
 * @brief 线性渐变原理
 */
class LinearGradientAlgorithmWidget : public QWidget {
    Q_OBJECT

public:
    explicit LinearGradientAlgorithmWidget(QWidget *parent = 0);
    ~LinearGradientAlgorithmWidget();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::LinearGradientAlgorithmWidget *ui;
};

#endif // LINEARGRADIENTALGORITHMWIDGET_H
