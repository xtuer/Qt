#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QImage>
#include <QPixmap>

namespace Ui {
class ImageWidget;
}

class ImageWidget : public QWidget {
    Q_OBJECT

public:
    explicit ImageWidget(QWidget *parent = 0);
    ~ImageWidget();

private:
    Ui::ImageWidget *ui;
    QImage  image;
    QPixmap pixmap;
};

#endif // IMAGEWIDGET_H
