#include "LinearGradientAlgorithmWidget.h"
#include "ui_LinearGradientAlgorithmWidget.h"
#include <QPainter>

LinearGradientAlgorithmWidget::LinearGradientAlgorithmWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::LinearGradientAlgorithmWidget) {
    ui->setupUi(this);
}

LinearGradientAlgorithmWidget::~LinearGradientAlgorithmWidget() {
    delete ui;
}

void LinearGradientAlgorithmWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 渐变填充的矩形
    QRect rect(20, 20, 200, 200);

    // 渐变开始和结束的颜色
    QColor gradientStartColor(255, 0, 0);
    QColor gradientFinalColor(0, 0, 255);

    int r1, g1, b1, r2, g2, b2;
    gradientStartColor.getRgb(&r1, &g1, &b1);
    gradientFinalColor.getRgb(&r2, &g2, &b2);

    qreal y1 = rect.y();
    qreal y2 = rect.y() + rect.height();

    // 计算矩形中每一个点的颜色，然后用此颜色绘制这个点
    for (int x = rect.x(); x <= rect.x() + rect.width(); ++x) {
        for (int y = rect.y(); y <= rect.y() + rect.height(); ++y) {
            qreal t = (y-y1) / (y2-y1);
            t = qMax(0.0, qMin(t, 1.0));

            int r = r1 + t * (r2-r1);
            int g = g1 + t * (g2-g1);
            int b = b1 + t * (b2-b1);

            painter.setPen(QColor(r, g, b));
            painter.drawPoint(x, y);
        }
    }
}
