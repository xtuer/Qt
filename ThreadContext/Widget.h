#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class Thread;

class Widget : public QWidget {
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();


    Q_INVOKABLE void directCall();
    Q_INVOKABLE void invokeCall();
    Q_INVOKABLE void slotCall();

private:
    Thread *thread;
};

#endif // WIDGET_H
