#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QLabel>

class MessageShower;

namespace Ui {
class MyWidget;
}

class MyWidget : public QWidget {
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = 0);
    ~MyWidget();

private:
    void initializeUi();
    void handleEvents();

    Ui::MyWidget *ui;
    MessageShower *messageShower;
};

#endif // MYWIDGET_H
