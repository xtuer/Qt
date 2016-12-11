#include "SmoothCurveWidget.h"
#include "ui_SmoothCurveWidget.h"
#include "SmoothCurveGenerator1.h"
#include "SmoothCurveGenerator2.h"

#include <QPainter>
#include <QtGlobal>
#include <QDateTime>

SmoothCurveWidget::SmoothCurveWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::SmoothCurveWidget) {
    ui->setupUi(this);

    connect(ui->generateCurveButton, SIGNAL(clicked(bool)), this, SLOT(generateCurves()));
    connect(ui->showKnotsCheckBox, SIGNAL(clicked(bool)), this, SLOT(update()));
    connect(ui->showSmoothCurveCheckBox, SIGNAL(clicked(bool)), this, SLOT(update()));
    connect(ui->smoothCurveGeneratorComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(update()));

    ui->generateCurveButton->click();
}

SmoothCurveWidget::~SmoothCurveWidget() {
    delete ui;
}

void SmoothCurveWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2 , height() / 2);
    painter.scale(1, -1);

    // 画坐标轴
    painter.setPen(QColor(180, 180, 180));
    painter.drawLine(-500, 0, 500, 0);
    painter.drawLine(0, 500, 0, -500);

    // showSmoothCurveCheckBox 被选中时显示平滑曲线，否则显示非平滑曲线
    painter.setPen(QPen(QColor(80, 80, 80), 2));
    if (ui->showSmoothCurveCheckBox->isChecked() && ui->smoothCurveGeneratorComboBox->currentIndex() == 0) {
        painter.drawPath(smoothCurve1);
    } else if (ui->showSmoothCurveCheckBox->isChecked() && ui->smoothCurveGeneratorComboBox->currentIndex() == 1) {
        painter.drawPath(smoothCurve2);
    } else {
        painter.drawPath(nonSmoothCurve);
    }

    // 如果曲线上的点可见，则显示出来
    if (ui->showKnotsCheckBox->isChecked()) {
        painter.setPen(Qt::black);
        painter.setBrush(Qt::gray);
        foreach(QPointF p, knots) {
            painter.drawEllipse(p, 3, 3);
        }
    }
}

void SmoothCurveWidget::generateCurves() {
    qsrand(QDateTime::currentDateTime().toMSecsSinceEpoch());

    // 随机生成曲线上的点: 横坐标为 [-200, 200]，纵坐标为 [-100, 100]
    int x = -200;
    knots.clear();
    while (x < 200) {
        knots << QPointF(x, qrand() % 200 - 100);
        x += qMin(qrand() % 30 + 5, 200);
    }

    // 根据曲线上的点创建平滑曲线
    smoothCurve1 = SmoothCurveGenerator1::generateSmoothCurve(knots);
    smoothCurve2 = SmoothCurveGenerator2::generateSmoothCurve(knots);

    // 连接点创建非平滑曲线曲线
    nonSmoothCurve = QPainterPath();
    nonSmoothCurve.moveTo(knots[0]);
    for (int i = 1; i < knots.size(); ++i) {
        nonSmoothCurve.lineTo(knots[i]);
    }

    update();
}
