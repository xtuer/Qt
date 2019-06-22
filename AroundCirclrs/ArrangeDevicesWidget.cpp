#include "ArrangeDevicesWidget.h"
#include "ui_ArrangeDevicesWidget.h"
#include "AroundCirclesWidget.h"
#include "AroundCirclesGraphicsView.h"

#include <QDebug>
#include <QLabel>
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>
#include <QRandomGenerator>

/*-----------------------------------------------------------------------------|
 |                         ArrangeDevicesWidgetPrivate                         |
 |----------------------------------------------------------------------------*/
class ArrangeDevicesWidgetPrivate {
    friend class ArrangeDevicesWidget;

    ArrangeDevicesWidgetPrivate();

    QPoint startDragPos;
    QList<QString> colors;
    AroundCirclesWidget *circlesWidget = nullptr;
    AroundCirclesGraphicsView *circlesView = nullptr;
};

ArrangeDevicesWidgetPrivate::ArrangeDevicesWidgetPrivate() {
    // 随机生成颜色，可以是指定的
    for (int i = 0; i < 100; ++i) {
        int r = QRandomGenerator::global()->bounded(256);
        int g = QRandomGenerator::global()->bounded(256);
        int b = QRandomGenerator::global()->bounded(256);
        colors << QColor(r, g, b).name(); // 十六进制的颜色: #aabbed
    }

    circlesWidget = new AroundCirclesWidget();
    circlesView = new AroundCirclesGraphicsView();
}

/*-----------------------------------------------------------------------------|
 |                            ArrangeDevicesWidget                             |
 |----------------------------------------------------------------------------*/
ArrangeDevicesWidget::ArrangeDevicesWidget(QWidget *parent) : QWidget(parent), ui(new Ui::ArrangeDevicesWidget) {
    ui->setupUi(this);

    d = new ArrangeDevicesWidgetPrivate();
    // layout()->replaceWidget(ui->placeHolderWidget, d->circlesWidget);
    layout()->replaceWidget(ui->placeHolderWidget, d->circlesView);

    ui->devicesWidget->layout()->addItem(new QSpacerItem(20, 424, QSizePolicy::Minimum, QSizePolicy::Expanding));

    // 添加 16 个设备，用于测试
    // 提示: 拖拽设备到某个圆上后，圆的名字自动设置为设备的名字，
    // 就可以调用 d->circlesView->setCircleBgcolor(deviceName, color) 设置圆的背景色了
    for (int i = 1; i <= 16; ++i) {
        addDevice(QString("Device-%1").arg(i), d->colors[i]);
    }
    ui->devicesWidget->layout()->addItem(new QSpacerItem(20, 424, QSizePolicy::Minimum, QSizePolicy::Expanding));

    this->setStyleSheet(QString(".DeviceLabel {border: 1px solid gray;}"));
}

ArrangeDevicesWidget::~ArrangeDevicesWidget() {
    delete ui;
    delete d;
}

bool ArrangeDevicesWidget::eventFilter(QObject *watched, QEvent *event) {
    QLabel *label = qobject_cast<QLabel *>(watched);

    if (nullptr == label) {
        return QObject::eventFilter(watched, event);
    }

    // 设备 label 的拖拽事件
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *me = dynamic_cast<QMouseEvent *>(event);
        d->startDragPos = me->globalPos();

        return true;
    } else if (event->type() == QEvent::MouseMove) {
        QMouseEvent *me = dynamic_cast<QMouseEvent *>(event);
        int distance = (me->globalPos() - d->startDragPos).manhattanLength();

        if (distance >= QApplication::startDragDistance()) {
            startDrag(label);
        }

        return true;
    }

    return QObject::eventFilter(watched, event);
}

// 开始拖拽设备 label
void ArrangeDevicesWidget::startDrag(QLabel *label) {
    QString color    = label->property("color").toString();
    QString deviceId = label->property("deviceId").toString();
    QString content  = QString("%1,%2").arg(deviceId).arg(color);
    QString mimeType = "text/DnD-DEVICE-CIRCLE";

    QByteArray contentBytes = content.toUtf8();
    QMimeData *mimeData = new QMimeData();
    mimeData->setData(mimeType, contentBytes);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);

    QPixmap pixmap = label->grab();
    drag->setPixmap(pixmap);
    QSize s = pixmap.size();
    drag->setHotSpot(QPoint(s.width()/2, s.height()/2));

#ifdef Q_OS_MAC
    drag->setHotSpot(QPoint(s.width()/4, s.height()/4)); // Mac 下需要调整为四分之一处
#endif

    drag->exec(Qt::CopyAction); // Start drag
}

// 添加设备
void ArrangeDevicesWidget::addDevice(const QString name, const QString color) {
    QLabel *label = new QLabel(name);
    label->setProperty("class", "DeviceLabel");
    label->setProperty("color", color);
    label->setProperty("deviceId", name);
    label->setStyleSheet(QString("background: %1").arg(color));
    label->installEventFilter(this);

    ui->devicesWidget->layout()->addWidget(label);
}
