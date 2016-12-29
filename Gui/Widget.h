#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>

namespace Ui {
class Widget;
}

class Widget : public QWidget {
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public slots:
     void foo() {
         qDebug() << "foo111()";
     }

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
