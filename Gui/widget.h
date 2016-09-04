#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

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

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:

private:
    void accessUrl();

    Ui::Widget *ui;
    QNetworkAccessManager *manager;
    int count;
};

#endif // WIDGET_H
