#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QList>
#include <functional>
#include <QFile>

namespace Ui {
class Widget;
}

class Widget : public QWidget {
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    void loadQss();

    Ui::Widget *ui;
    QFile file;
};

#endif // WIDGET_H
