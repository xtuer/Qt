#ifndef ROUNDBUTTONWIDGET_H
#define ROUNDBUTTONWIDGET_H

#include <QWidget>

namespace Ui {
class RoundButtonWidget;
}

class RoundButtonWidget : public QWidget {
    Q_OBJECT

public:
    explicit RoundButtonWidget(QWidget *parent = 0);
    ~RoundButtonWidget();

private:
    Ui::RoundButtonWidget *ui;
};

#endif // ROUNDBUTTONWIDGET_H
