#ifndef DRAWSIMPLETEXTWIDGET_H
#define DRAWSIMPLETEXTWIDGET_H

#include <QWidget>

namespace Ui {
class DrawSimpleTextWidget;
}

/**
 * @brief 简单绘制文本
 */
class DrawSimpleTextWidget : public QWidget {
    Q_OBJECT

public:
    explicit DrawSimpleTextWidget(QWidget *parent = 0);
    ~DrawSimpleTextWidget();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::DrawSimpleTextWidget *ui;
};

#endif // DRAWSIMPLETEXTWIDGET_H
