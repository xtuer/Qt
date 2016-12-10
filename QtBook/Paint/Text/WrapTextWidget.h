#ifndef WRAPTEXTWIDGET_H
#define WRAPTEXTWIDGET_H

#include <QWidget>

namespace Ui {
class WrapTextWidget;
}

/**
 * @brief 换行绘制文本
 */
class WrapTextWidget : public QWidget {
    Q_OBJECT

public:
    explicit WrapTextWidget(QWidget *parent = 0);
    ~WrapTextWidget();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::WrapTextWidget *ui;
};

#endif // WRAPTEXTWIDGET_H
