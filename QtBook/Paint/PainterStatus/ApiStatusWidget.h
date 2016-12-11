#ifndef APISTATUSWIDGET_H
#define APISTATUSWIDGET_H

#include <QWidget>

namespace Ui {
class ApiStatusWidget;
}

class ApiStatusWidget : public QWidget {
    Q_OBJECT

public:
    explicit ApiStatusWidget(QWidget *parent = 0);
    ~ApiStatusWidget();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::ApiStatusWidget *ui;
};

#endif // APISTATUSWIDGET_H
