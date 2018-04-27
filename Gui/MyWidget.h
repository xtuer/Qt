#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QDebug>

class MessageShower;

namespace Ui {
class MyWidget;
}

class MyWidget : public QWidget {
    Q_OBJECT

    Q_PROPERTY(QString title READ title WRITE setTitle USER true)

public:
    explicit MyWidget(QWidget *parent = 0);
    ~MyWidget();

    QString title() const { return ""; }
    void setTitle(const QString &title) {
        qDebug() << "setTitle";
    }

private:
    void initializeUi();
    void handleEvents();

    Ui::MyWidget *ui;
    MessageShower *messageShower;
};

#endif // MYWIDGET_H
