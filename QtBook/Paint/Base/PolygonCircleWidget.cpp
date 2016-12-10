#include "PolygonCircleWidget.h"
#include "ui_PolygonCircleWidget.h"
#include <QPainter>
#include <QtMath>

PolygonCircleWidget::PolygonCircleWidget(QWidget *parent) : QWidget(parent), ui(new Ui::PolygonCircleWidget) {
    ui->setupUi(this);
}

PolygonCircleWidget::~PolygonCircleWidget() {
    delete ui;
}

void PolygonCircleWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // 启用抗锯齿效果
    painter.translate(width() / 2, height() / 2);  // 把坐标原点移动到 widget 的中心

    const int COUNT  = 15;  // 边数，越大就越像圆
    const int RADIUS = 100; // 圆的半径
    QPointF points[COUNT];  // 顶点数组

    for (int i = 0; i < COUNT; ++i) {
        float radians = 2 * M_PI / COUNT * i; // M_PI 是 QtMath 里定义的，就是 PI
        float x = RADIUS * qCos(radians);
        float y = RADIUS * qSin(radians);
        points[i] = QPointF(x, y);
    }

    painter.setBrush(Qt::lightGray);
    painter.drawPolygon(points, COUNT);
}
