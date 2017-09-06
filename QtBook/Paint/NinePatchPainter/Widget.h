#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPixmap>

class NinePatchPainter;

class Widget : public QWidget {
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    QPixmap pixmap;
    NinePatchPainter *ninePatchPainter;
};

#endif // WIDGET_H
