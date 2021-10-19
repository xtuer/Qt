#include "ArrangeDevicesWidget.h"
#include "ui_ArrangeDevicesWidget.h"
#include "AroundDevicesGraphicsView.h"
#include "PixmapDevicesGraphicsView.h"
#include "RectDevicesGraphicsView.h"
#include "Rect16DevicesGraphicsView.h"
#include "Rect16DevicesGraphicsView_v1.h"
#include "Rect3BlocksDevicesGraphicsView.h"
#include "DeviceItems.h"

#include <QDebug>
#include <QLabel>
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>

// 默认定义的 16 种颜色
static QList<QString> colors = {
    "#00FFCC", "#00FFFF", "#00CCCC", "#00CCFF", "#00FF99", "#CCCC33", "#99CCFF", "#CCFF66",
    "#CCFFCC", "#FF99FF", "#FF9900", "#FF6600", "#FF9999", "#FFFF99", "#DDDDDD", "#FF9900",
};

/*-----------------------------------------------------------------------------|
 |                         ArrangeDevicesWidgetPrivate                         |
 |----------------------------------------------------------------------------*/
class ArrangeDevicesWidgetPrivate {
    friend class ArrangeDevicesWidget;

    ArrangeDevicesWidgetPrivate();

    QPoint startDragPos;
//    AroundDevicesGraphicsView *circleDevicesView = nullptr;
//    PixmapDevicesGraphicsView *pixmapDevicesView = nullptr;
//    RectDevicesGraphicsView *rectDevicesView = nullptr;
//    Rect16DevicesGraphicsView *rect16DevicesView = nullptr;
//    Rect3BlocksDevicesGraphicsView *rect3BlocksDevicesGraphicsView = nullptr;
    QGraphicsView *graphicsView = nullptr;
};

ArrangeDevicesWidgetPrivate::ArrangeDevicesWidgetPrivate() {
//    circleDevicesView = new AroundDevicesGraphicsView();
//    pixmapDevicesView = new PixmapDevicesGraphicsView();
    // rectDevicesView = new RectDevicesGraphicsView();
}

/*-----------------------------------------------------------------------------|
 |                            ArrangeDevicesWidget                             |
 |----------------------------------------------------------------------------*/
ArrangeDevicesWidget::ArrangeDevicesWidget(int type, const QStringList &deviceNames, QWidget *parent)
    : QWidget(parent), ui(new Ui::ArrangeDevicesWidget), d(new ArrangeDevicesWidgetPrivate) {
    ui->setupUi(this);

    // 根据 type 确定布点类型
    if (1 == type) {
        d->graphicsView = new AroundDevicesGraphicsView();
        layout()->replaceWidget(ui->placeHolderWidget, d->graphicsView);
    } else if (2 == type) {
        d->graphicsView = new PixmapDevicesGraphicsView();
        layout()->replaceWidget(ui->placeHolderWidget, d->graphicsView);
    } else if (3 == type) {

    } else if (4 == type) {
        d->graphicsView = new Rect16DevicesGraphicsView();
        layout()->replaceWidget(ui->placeHolderWidget, d->graphicsView);
    } else if (5 == type) {
        d->graphicsView = new Rect3BlocksDevicesGraphicsView();
        layout()->replaceWidget(ui->placeHolderWidget, d->graphicsView);
    } else if (6 == type) {
        d->graphicsView = new Rect16DevicesGraphicsView_v1();
        layout()->replaceWidget(ui->placeHolderWidget, d->graphicsView);
    }

    // 创建设备列表
    ui->devicesWidget->layout()->addItem(new QSpacerItem(20, 424, QSizePolicy::Minimum, QSizePolicy::Expanding));
    for (int i = 0; i < deviceNames.size(); ++i) {
        addDevice(deviceNames.at(i), colors[i%colors.size()]);
    }
    ui->devicesWidget->layout()->addItem(new QSpacerItem(20, 424, QSizePolicy::Minimum, QSizePolicy::Expanding));

    this->setStyleSheet(QString(".DeviceLabel { border: 1px solid gray; }"));

    // 点击按钮修改设备显示的值
    connect(this->ui->changeDeviceValueButton, &QPushButton::clicked, [this] {
        DeviceItem::setValueByName(d->graphicsView->scene(), "Device-1", "1");
    });
}

ArrangeDevicesWidget::~ArrangeDevicesWidget() {
    delete ui;
    delete d;
}

// 初始化布点图类型 3 (矩形布点图)
void ArrangeDevicesWidget::initMode3(int horizontalCount, int verticalCount) {
    d->graphicsView = new RectDevicesGraphicsView(horizontalCount, verticalCount);
    layout()->replaceWidget(ui->placeHolderWidget, d->graphicsView);
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
    label->setStyleSheet(QString("background: %1; border-radius: 3px; padding: 3px").arg(color));
    label->installEventFilter(this);

    ui->devicesWidget->layout()->addWidget(label);
}
