#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QAtomicInt>

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget {
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private:
    Ui::MainWidget *ui;
    QAtomicInt connectedCount = 0;
};

#endif // MAINWIDGET_H
