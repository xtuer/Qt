#ifndef OXYWIDGET_H
#define OXYWIDGET_H

#include <QWidget>

namespace Ui {
class OxyWidget;
}

class OxyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OxyWidget(QWidget *parent = 0);
    ~OxyWidget();

private:
    Ui::OxyWidget *ui;
};

#endif // OXYWIDGET_H
