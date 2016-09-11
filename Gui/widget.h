#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QList>
#include <functional>

class QNetworkReply;
class QNetworkAccessManager;
class QRubberBand;

namespace Ui {
class Widget;
}

class Widget : public QWidget {
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void foo(std::function<void (int)> handler);
private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
