#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QMutex>

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
    void oneRequestFinish();

    Ui::MainWidget *ui;
    int totalCount;
    int finishedCount;
    QNetworkAccessManager * networkAccessManager;
    QMutex mutex;
};

#endif // MAINWIDGET_H
