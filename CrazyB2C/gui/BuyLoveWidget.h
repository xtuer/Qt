#ifndef BUYLOVEWIDGET_H
#define BUYLOVEWIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QMutex>

namespace Ui {
class BuyLoveWidget;
}

class BuyLoveWidget : public QWidget {
    Q_OBJECT

public:
    explicit BuyLoveWidget(QWidget *parent = 0);
    ~BuyLoveWidget();

private:
    void increase();
    void oneRequestFinish();

    Ui::BuyLoveWidget *ui;
    int totalCount;
    int finishedCount;
    QNetworkAccessManager * networkAccessManager;
    QMutex mutex;
};

#endif // BUYLOVEWIDGET_H
