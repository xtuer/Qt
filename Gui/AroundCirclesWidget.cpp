#include "AroundCirclesWidget.h"
#include "ui_AroundCirclesWidget.h"

#include <QDebug>
#include <QPainter>
#include <QtMath>
#include <QRectF>

AroundCirclesWidget::AroundCirclesWidget(QWidget *parent) : QWidget(parent), ui(new Ui::AroundCirclesWidget) {
    ui->setupUi(this);

    // 初始化 100 个无效颜色
    for (int i = 0; i < 100; ++i) {
        colors << QColor();
    }
}

AroundCirclesWidget::~AroundCirclesWidget() {
    delete ui;
}

void AroundCirclesWidget::paintEvent(QPaintEvent *) {
    // 1. 移动坐标原点到 widget 的中心
    // 2. 计算小圆的半径
    // 3. 绘制中心的大圆
    // 4. 绘制大圆上的序号: 逆时针，1 到 n
    // 5. 绘制第一圈绕大圆的小圆
    // 6. 绘制第二圈绕大圆的小圆

    // [1] 移动坐标原点到 widget 的中心
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.translate(width() / 2, height() / 2);

    double sr = aroundCircleRadius(cr, n); // [2] 计算小圆的半径
    double br = cr+8;                      // 中心大圆的半径: +8 调整效果

    // [1] 绘制中心的大圆
    painter.drawEllipse(QRectF(-br, -br, 2*br, 2*br));

    // [4] 绘制大圆上的序号: 逆时针，1 到 n
    for (int i = 1; i <= n; ++i) {
        painter.save();
        painter.rotate(-360.0/n*i);
        painter.translate(0, cr-5);
        painter.drawText(QRectF(-8, -8, 16, 16), Qt::AlignCenter, QString::number(i));
        painter.restore();
    }

    int sn = 1; // 小圆的序号，用于查找颜色

    // [5] 绘制第一圈绕大圆的小圆
    int n1 = n;
    for (int i = 0; i < n1; ++i) {
        painter.save();
        painter.rotate(-360.0/n1*i);
        painter.translate(0, cr+sr+15);

        painter.setBrush(getColor(sn));
        painter.drawEllipse(QRectF(-sr, -sr, 2*sr, 2*sr));
        painter.drawText(QRectF(-sr, -sr, 2*sr, 2*sr), Qt::AlignCenter, QString::number(sn++));

        painter.restore();
    }

    // [6] 绘制第二圈绕大圆的小圆
    double br2 = cr+sr*3;
    int n2 = int(M_PI*br2 / sr);
    for (int i = 0; i < n2; ++i) {
        painter.save();
        painter.rotate(-360.0/n2*i);
        painter.translate(0, br2+20);

        painter.setBrush(getColor(sn));
        painter.drawEllipse(QRectF(-sr, -sr, 2*sr, 2*sr));
        painter.drawText(QRectF(-sr, -sr, 2*sr, 2*sr), Qt::AlignCenter, QString::number(sn++));

        painter.restore();
    }
}

double AroundCirclesWidget::aroundCircleRadius(double r, int n) const {
    // 圆的内切正 n 边型的边长为 2*r*sin(π/n)
    // 小圆的半径为 x, 计算半径为 r+x 的圆的 2n 个内切正多边形的边长 b, b 等于 x
    double t = qSin(M_PI/2/n);
    return (2*r*t) / (1-2*t);
}

// 设置第 sn 个小圆的颜色
void AroundCirclesWidget::setColor(int sn, const QColor color) {
    if (sn >= 0 && sn < colors.size()) {
        colors[sn] = color;
        update();
    }
}

// 获取第 sn 个小圆的颜色
QColor AroundCirclesWidget::getColor(int sn) const {
    QColor color = colors.value(sn);

    return color.isValid() ? color : Qt::transparent;
}
