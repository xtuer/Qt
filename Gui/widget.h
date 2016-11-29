#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QList>
#include <functional>
#include <QFile>

struct Item;

namespace Ui {
class Widget;
}

class Widget : public QWidget {
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    void loadQss();

    Ui::Widget *ui;

    QList<Item*> items;
};

#endif // WIDGET_H
