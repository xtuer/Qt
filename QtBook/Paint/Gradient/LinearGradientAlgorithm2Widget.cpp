#include "LinearGradientAlgorithm2Widget.h"
#include "ui_LinearGradientAlgorithm2Widget.h"
#include <QPainter>
#include <QtMath>

LinearGradientAlgorithm2Widget::LinearGradientAlgorithm2Widget(QWidget *parent) :
    QWidget(parent), ui(new Ui::LinearGradientAlgorithm2Widget) {
    ui->setupUi(this);
}

LinearGradientAlgorithm2Widget::~LinearGradientAlgorithm2Widget() {
    delete ui;
}

void LinearGradientAlgorithm2Widget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 渐变填充的矩形
    QRect rect(20, 20, 200, 200);

    // 渐变开始和结束的颜色、位置
    QColor gradientStartColor(255, 0, 0);
    QColor gradientFinalColor(0, 0, 255);
    QPoint gradientStartPoint(60, 60);
    QPoint gradientFinalPoint(180, 180);

    // 颜色分量
    int r1, g1, b1, r2, g2, b2;
    gradientStartColor.getRgb(&r1, &g1, &b1);
    gradientFinalColor.getRgb(&r2, &g2, &b2);

    qreal dx = gradientFinalPoint.x() - gradientStartPoint.x();
    qreal dy = gradientFinalPoint.y() - gradientStartPoint.y();
    qreal length = qSqrt(dx*dx + dy*dy); // 渐变开始和结束的线段的长度
    float radian = qAtan2(dy, dx); // 渐变方向和 X 轴的夹角

    // 先移动，后旋转，要先调用旋转的函数，然后在调用移动的函数，一定要注意这点，
    // 因为底层实现是 matrix 矩阵右乘点的坐标的列矩阵
    QTransform transform;
    transform.rotateRadians(-radian);
    transform.translate(-gradientStartPoint.x(), -gradientStartPoint.y());

    // 计算矩形中每一个点的颜色，然后用此颜色绘制这个点
    for (int x = rect.x(); x <= rect.x() + rect.width(); ++x) {
        for (int y = rect.y(); y <= rect.y() + rect.height(); ++y) {
            QPointF p = transform.map(QPointF(x ,y));
            qreal t = p.x() / length;
            t = qMax(0.0, qMin(t, 1.0));

            int r = r1 + t * (r2-r1);
            int g = g1 + t * (g2-g1);
            int b = b1 + t * (b2-b1);

            painter.setPen(QColor(r, g, b));
            painter.drawPoint(x, y);
        }
    }
}
