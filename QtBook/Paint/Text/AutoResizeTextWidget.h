#ifndef AUTORESIZETEXTWIDGET_H
#define AUTORESIZETEXTWIDGET_H

#include <QWidget>

namespace Ui {
class AutoResizeTextWidget;
}

/**
 * @brief 文本的包围矩形，自动计算文本占用的矩形区
 */
class AutoResizeTextWidget : public QWidget {
    Q_OBJECT

public:
    explicit AutoResizeTextWidget(QWidget *parent = 0);
    ~AutoResizeTextWidget();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::AutoResizeTextWidget *ui;
};

#endif // AUTORESIZETEXTWIDGET_H
