#ifndef GRIDWIDGET_H
#define GRIDWIDGET_H

#include <QWidget>

namespace Ui {
class GridWidget;
}

/**
 * @brief 使用画线来画网格
 */
class GridWidget : public QWidget {
    Q_OBJECT

public:
    explicit GridWidget(QWidget *parent = 0);
    ~GridWidget();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::GridWidget *ui;
};

#endif // GRIDWIDGET_H
