#ifndef PAINTBUTTONWIDGET_H
#define PAINTBUTTONWIDGET_H

#include <QWidget>

namespace Ui {
class PaintButtonWidget;
}

class PaintButtonWidget : public QWidget {
    Q_OBJECT

public:
    explicit PaintButtonWidget(QWidget *parent = 0);
    ~PaintButtonWidget();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::PaintButtonWidget *ui;
};

#endif // PAINTBUTTONWIDGET_H
