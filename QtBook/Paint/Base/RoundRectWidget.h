#ifndef ROUNDRECTWIDGET_H
#define ROUNDRECTWIDGET_H

#include <QWidget>

namespace Ui {
class RoundRectWidget;
}

/**
 * @brief 圆角矩形
 */
class RoundRectWidget : public QWidget {
    Q_OBJECT

public:
    explicit RoundRectWidget(QWidget *parent = 0);
    ~RoundRectWidget();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::RoundRectWidget *ui;
};

#endif // ROUNDRECTWIDGET_H
