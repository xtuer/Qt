#include "LinearGradientWidget.h"
#include "ui_LinearGradientWidget.h"
#include <QPainter>
#include <QLinearGradient>

LinearGradientWidget::LinearGradientWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::LinearGradientWidget) {
    ui->setupUi(this);
}

LinearGradientWidget::~LinearGradientWidget() {
    delete ui;
}

void LinearGradientWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect rect(20, 20, 200, 200);

    // 渐变开始的坐标为 (20, 20), 结束的坐标为 (20, 220)
    QLinearGradient gradient(rect.x(), rect.y(),
                             rect.x(), rect.y() + rect.height());
    gradient.setColorAt(0.0, Qt::red);
    gradient.setColorAt(1.0, Qt::blue);
    gradient.setSpread(QGradient::ReflectSpread);

    // 超出渐变范围后的填充方式
    painter.setPen(Qt::NoPen);
    painter.setBrush(gradient); // QBrush(const QGradient &gradient)
    painter.drawRect(rect);
}
