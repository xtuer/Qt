#ifndef MULTIPLELINESWIDGET_H
#define MULTIPLELINESWIDGET_H

#include <QWidget>

namespace Ui {
class MultipleLinesWidget;
}

/**
 * @brief 画多条线段
 */
class MultipleLinesWidget : public QWidget {
    Q_OBJECT

public:
    explicit MultipleLinesWidget(QWidget *parent = 0);
    ~MultipleLinesWidget();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::MultipleLinesWidget *ui;
};

#endif // MULTIPLELINESWIDGET_H
