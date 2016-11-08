#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class XThread;
class QLabel;

class Widget : public QWidget {
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    Q_INVOKABLE void showTime(const QString &time);

private:
    XThread *thread;
    QLabel *timeLabel;
};

#endif // WIDGET_H
