#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QList>

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
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private slots:

private:

    Ui::Widget *ui;
    QNetworkAccessManager *manager;
    int count;

    QList<double*> ns;
};

#endif // WIDGET_H
