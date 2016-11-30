#ifndef CAROUSEL_H
#define CAROUSEL_H

#include <QWidget>
#include <QList>

struct CarouselItem;

class Carousel : public QWidget {
    Q_OBJECT

public:
    explicit Carousel(QWidget *parent = 0);
    ~Carousel();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    QList<CarouselItem*> items;
};

#endif // CAROUSEL_H
