#ifndef FOXWIDGET_H
#define FOXWIDGET_H

#include <QWidget>

class FoxWidget : public QWidget {
    Q_OBJECT
public:
    explicit FoxWidget(QWidget *parent = 0);
    ~FoxWidget();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
};

#endif // FOXWIDGET_H
