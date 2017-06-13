#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QAtomicInteger>

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget {
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

private:
    void increase();

    Ui::MainWidget *ui;
    int totalCount;
    QAtomicInt::QAtomicInteger finishedCount;
    QNetworkAccessManager * networkAccessManager;
};

#endif // MAINWIDGET_H
