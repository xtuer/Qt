#ifndef PIXMAPWIDGET_H
#define PIXMAPWIDGET_H

#include <QWidget>

namespace Ui {
class PixmapWidget;
}

/**
 * @brief 绘制 Pixmap，可以缩放绘制，绘制局部，也可以重复绘制
 */
class PixmapWidget : public QWidget {
    Q_OBJECT

public:
    explicit PixmapWidget(QWidget *parent = 0);
    ~PixmapWidget();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::PixmapWidget *ui;
};

#endif // PIXMAPWIDGET_H
