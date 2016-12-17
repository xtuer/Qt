#ifndef CLIPWIDGET_H
#define CLIPWIDGET_H

#include <QWidget>

class ClipWidget : public QWidget {
    Q_OBJECT

public:
    explicit ClipWidget(QWidget *parent = 0);
    ~ClipWidget();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
};

#endif // CLIPWIDGET_H
