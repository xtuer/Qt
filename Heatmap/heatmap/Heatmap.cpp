#include "Heatmap.h"
#include "GradientPalette.h"

#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QColor>
#include <QPainter>
#include <QRadialGradient>
#include <QLinearGradient>
#include <QtGlobal>

// 创建热力图对象
Heatmap::Heatmap(int width, int height, qreal max, int radius, int opacity) {
    this->width   = width;
    this->height  = height;
    this->radius  = qMax(5, radius);
    this->opacity = qMax(0, qMin(opacity, 255)); // [0, 255]
    this->max     = max;

    data.resize(width * height);
    data.fill(0);
	
    mainCanvas  = new QImage(width, height, QImage::Format_ARGB32);
    alphaCanvas = new QImage(width, height, QImage::Format_ARGB32);

    // 调色板的 stops
    QLinearGradient gradient;
    gradient.setColorAt(0.45, Qt::blue);
    gradient.setColorAt(0.55, Qt::cyan);
    gradient.setColorAt(0.65, Qt::green);
    gradient.setColorAt(0.85, Qt::yellow);
    gradient.setColorAt(1.00, Qt::red);
    setGradient(gradient);
}

Heatmap::~Heatmap() {
    delete palette;
    delete mainCanvas;
    delete alphaCanvas;
}

// 获取热力图的图片
QPixmap Heatmap::getHeatmap() {
    draw();
    return QPixmap::fromImage(*mainCanvas);
}

// 保存热力图
void Heatmap::save(const QString &path) {
    getHeatmap().save(path);
}

void Heatmap::setGradient(const QLinearGradient &gradient) {
    delete palette;
    palette = new GradientPalette(gradient, width);
}

// 热力图上增加点
void Heatmap::addPoint(int x, int y, qreal value) {
    if (x < 0 || y < 0 || x >= width || y >= height) {
        return;
    }

    data[y * width + x] = value;
}

/*
 * 重载方法，着色
 */
void Heatmap::draw() {
    // 绘制 Alpha 圆圈通道
    alphaCanvas->fill(QColor(0, 0, 0, 0));

    int size = data.size();
    for (int i = 0; i < size; ++i) {
        if (data[i] > 0) {
            drawAlpha(i%width, i/width, data[i]);
        }
    }

    mainCanvas->fill(QColor(0, 0, 0, 0));
    colorize(0, 0, width, height);
}

/*
 * 绘制透明径向渐变
 *
 * @param x 横坐标
 * @param y 纵坐标
 * @param count 被命中次数
 * @param colorize_now 是否调用着色方法
 */
void Heatmap::drawAlpha(int x, int y, qreal value) {
    int alpha = int(qreal(value / max)*255);
    QRadialGradient gradient(x, y, radius);
    gradient.setColorAt(0, QColor(0, 0, 0, alpha));
    gradient.setColorAt(1, QColor(0, 0, 0, 0));

    QPainter painter(alphaCanvas);
    painter.setPen(Qt::NoPen);
    painter.setBrush(gradient);
    painter.drawEllipse(QPoint(x, y), radius, radius);
}

/*
 * 重载函数，实际的着色操作在本方法
 *
 * @param left   左上角横坐标
 * @param top    左上角纵坐标
 * @param right  右下角横坐标
 * @param bottom 右下角纵坐标
 */
void Heatmap::colorize(int left, int top, int right, int bottom) {
	int alpha = 0;
	int finalAlpha = 0;
	QColor color;
    QRgb finalColor;

    for (int y = left; y < right; ++y) {
        for (int x = top; x < bottom; ++x) {
            alpha = qAlpha(alphaCanvas->pixel(y, x));

            // alpha 为 0 则不进行着色
            if (!alpha) {
				continue;
            }

            color = palette->getColorAt(alpha);
            finalAlpha = qMin(alpha, opacity);
            finalColor = qRgba(color.red(), color.green(), color.blue(), finalAlpha);
            mainCanvas->setPixel(y, x, finalColor);
		}
	}
}
