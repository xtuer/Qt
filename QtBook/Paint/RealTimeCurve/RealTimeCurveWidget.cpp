#include "RealTimeCurveWidget.h"
#include "ui_RealTimeCurveWidget.h"
#include "SmoothCurveGenerator2.h"

#include <QTimerEvent>
#include <QtGlobal>
#include <QDateTime>
#include <QPainter>
#include <QPainterPath>

RealTimeWidget::RealTimeWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::RealTimeCurveWidget) {
    ui->setupUi(this);

    w = 0;
    h = 0;
    maxSize = 30;   // 只存储最新的 30 个数据
    maxValue = 100; // 数据的最大值为 100，因为我们生成的随机数为 [0, 100]
    timerId = startTimer(200);
    qsrand(QDateTime::currentDateTime().toTime_t());

    connect(ui->showSmoothCurveCheckBox, SIGNAL(clicked(bool)), this, SLOT(update()));
}

RealTimeWidget::~RealTimeWidget() {
    delete ui;
}

void RealTimeWidget::timerEvent(QTimerEvent *event) {
    if (event->timerId() == timerId) {
        // 模拟不停的接收到新数据
        int newData = qrand() % (maxValue + 1);
        dataReceived(newData);
    }
}

void RealTimeWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    w = width() - 20;  // 数据的曲线所在矩形的宽
    h = height() - 20; // 数据的曲线所在矩形的宽

    painter.translate(10, h + 10); // 移动坐标原点到窗口左下角
    painter.scale(1, -1); // 旋转坐标系，使得 Y 轴正向朝上

    painter.drawRect(0, 0, w, h);    // 曲线所在的显示范围
    painter.setClipRect(0, 0, w, h); // 超出指定范围的内容不显示
    painter.drawPath(curve);
}

void RealTimeWidget::dataReceived(int value) {
    data << value;

    // 数据个数超过了指定值，则删除最先接收到的数据
    while (data.size() > maxSize) {
        data.removeFirst();
    }

    if (isVisible()) {
        // 界面被隐藏后就没有必要绘制数据的曲线了
        createCurve();
        update();
    }
}

void RealTimeWidget::createCurve() {
    QList<QPointF> knots = createCurveKnots(); // 生成曲线上点的坐标

    if (ui->showSmoothCurveCheckBox->isChecked()) {
        // 使用平滑曲线显示数据
        curve = SmoothCurveGenerator2::generateSmoothCurve(knots);
    } else {
        // 使用非平滑曲线显示数据
        curve = QPainterPath(knots[0]);
        for (int i = 1; i < knots.size(); ++i) {
            curve.lineTo(knots[i]);
        }
    }
}

QList<QPointF> RealTimeWidget::createCurveKnots() const {
    int length = data.size();
    double stepX = w / (maxSize - 1.0);  // X 轴方向 2 个点之间的距离
    double stepY = h / (double) maxValue;         // 数据单位 1 对应 Y 轴 stepY 个像素
    double x = w - stepX * (length - 1); // 第一个数据的 x 坐标
    QList<QPointF> knots; // 曲线上的点

    // 生成数据对应曲线上的点
    for (int i = 0; i < length; ++i, x += stepX) {
        knots << QPointF(x, data[i] * stepY);
    }

    return knots;
}
