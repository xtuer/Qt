#include "Widget.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QKeyEvent>

Widget::Widget(QWidget *parent) : QGraphicsView(parent) {
    // 随机定义 100 个颜色
    QColor *colors = new QColor[100];
    for (int i = 0; i < 100; ++i) {
        colors[i] = QColor(qrand() % 256, qrand() % 256, qrand() % 256, qrand() % 256);
    }

    QGraphicsScene *scene = new QGraphicsScene(this);
    QGraphicsItem   *item = NULL;

    // 创建 rowCount * colCount 个 items
    const int rowCount = 100;
    const int colCount = 1000;
    for (int row = 0; row < rowCount; row += 1) {
        for (int col = 0; col < colCount; col += 1) {
            item = scene->addRect(col << 5, row << 5, 24, 24, QPen(Qt::darkGray), QBrush(colors[qrand() % 100]));
            item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
        }
    }

    this->setScene(scene);
    this->setDragMode(QGraphicsView::RubberBandDrag);
    this->setRenderHint(QPainter::Antialiasing);

    delete[] colors;
}

Widget::~Widget() {

}

void Widget::keyPressEvent(QKeyEvent *event) {
    // A 左旋
    // D 右旋
    // W 放大
    // S 缩小
    switch (event->key()) {
    case Qt::Key_A:
        rotate(-3);
        break;
    case Qt::Key_D:
        rotate(3);
        break;
    case Qt::Key_W:
        scale(1.1, 1.1);
        break;
    case Qt::Key_S:
        scale(1/1.1, 1/1.1);
        break;
    }

    event->accept();
}


