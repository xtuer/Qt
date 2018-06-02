#include "Widget.h"
#include "ui_Widget.h"
#include "ScatterMap.h"
#include "Scatter.h"

#include "heatmap/heatmapper.h"
#include "heatmap/gradientpalette.h"
#include <QRadialGradient>
#include <QImage>
#include <QMouseEvent>

#include <QDebug>
#include <QPixmap>
#include <QGridLayout>
#include <QDateTime>
#include <QRandomGenerator>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    initialize();
    handleEvents();

    // TODO: 模拟创建一些随机的点，可以是从文件读取的
    QRandomGenerator random(QDateTime::currentDateTime().toMSecsSinceEpoch());
    for (int i = 0; i < 30; ++i) {
        scatterMap->addScatter(random.bounded(0.8), random.bounded(0.8));
    }
}

Widget::~Widget() {
    delete ui;
}

// 初始化
void Widget::initialize() {
    ui->setupUi(this);

    // 创建布点地图，设置地图的宽高
    scatterMap = new ScatterMap();
    scatterMap->setScatterMapWidth(ui->widthLineEdit->text().trimmed().toInt());
    scatterMap->setScatterMapHeight(ui->heightLineEdit->text().trimmed().toInt());

    // 把 scatterMap 放到界面中去，替换 ui->placeholderWidget
    QGridLayout *l = qobject_cast<QGridLayout *>(layout());
    delete l->replaceWidget(ui->placeholderWidget, scatterMap);
    ui->placeholderWidget->deleteLater();
    ui->placeholderWidget = 0;
}

// 信号槽事件处理
void Widget::handleEvents() {
    // 点击按钮时获取所有点的坐标
    connect(ui->pushButton, &QPushButton::clicked, [this] {
        scatterMap->setScatterMapWidth(ui->widthLineEdit->text().trimmed().toInt());
        scatterMap->setScatterMapHeight(ui->heightLineEdit->text().trimmed().toInt());

        qDebug() << scatterMap->getScatterPositions();
    });

    // 点 scatter 的坐标变化时，显示坐标
    connect(scatterMap, &ScatterMap::scatterPositionChanged, [this] (Scatter *scatter, QPoint pos) {
        Q_UNUSED(scatter)
        ui->infoLabel->setText(QString("[X: %1, Y: %2]").arg(pos.x()).arg(pos.y()));
    });

    // 更新布点地图的大小
    connect(ui->widthLineEdit, &QLineEdit::textChanged, [this] {
        scatterMap->setScatterMapWidth(ui->widthLineEdit->text().trimmed().toInt());
    });

    connect(ui->heightLineEdit, &QLineEdit::textChanged, [this] {
        scatterMap->setScatterMapHeight(ui->heightLineEdit->text().trimmed().toInt());
    });

    // 点击截图按钮进行截图
    connect(ui->captureButton, &QPushButton::clicked, [this] {
        QPixmap pixmap = scatterMap->grab();
        pixmap.save("scatter.png");
    });

    // 点击保存热力图
    connect(ui->heatMapButton, &QPushButton::clicked, [this] {
        int w = scatterMap->width();
        int h = scatterMap->height();

        GradientPalette palette(w);
        palette.setColorAt(0.45, Qt::blue);
        palette.setColorAt(0.55, Qt::cyan);
        palette.setColorAt(0.65, Qt::green);
        palette.setColorAt(0.85, Qt::yellow);
        palette.setColorAt(1.0, Qt::red);

        QImage heatMapCanvas(w, h, QImage::Format_ARGB32);
        heatMapCanvas.fill(QColor(0, 0, 0, 0));
        HeatMapper heatMapper(&heatMapCanvas, &palette, 60, 128);

        for (QPoint p : scatterMap->getScatterPositionsInParentWidget()) {
            heatMapper.addPoint(p.x(), p.y());
        }

        heatMapper.save("heatmap.png");

        ui->heatMapLabel->setPixmap(QPixmap::fromImage(heatMapCanvas));
    });
}
