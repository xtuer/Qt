#include "ColourfulDevicesWidget.h"
#include "ui_ColourfulDevicesWidget.h"
#include "ColourfulDeviceWidget.h"

#include <QDebug>
#include <QGraphicsBlurEffect>
#include <QGraphicsDropShadowEffect>

ColourfulDevicesWidget::ColourfulDevicesWidget(int columnCountOfDevice, QWidget *parent)
    : QWidget(parent), ui(new Ui::ColourfulDevicesWidget), columnCountOfDevice(columnCountOfDevice) {
    initialize();
    handleEvents();
}

ColourfulDevicesWidget::~ColourfulDevicesWidget() {
    delete ui;
}

void ColourfulDevicesWidget::initialize() {
    ui->setupUi(this);
    this->layout()->setSizeConstraint(QLayout::SetFixedSize);
}

void ColourfulDevicesWidget::handleEvents() {

}

// 使用设备的名字初始化设备列表
void ColourfulDevicesWidget::createDevices(QList<QString> deviceNames) {
    // 清空设备 widgets
    emptyWidget(ui->devicesWidget);
    devices.clear();

    // 创建设备
    for (const QString &name : deviceNames) {
        devices.append(new ColourfulDeviceWidget(name));
    }

    // 放置设备
    placeDevices();
}

// 设置名字为 deviceName 的设备的颜色
void ColourfulDevicesWidget::setDeviceColor(const QString &deviceName, const QColor &color) {
    QList<ColourfulDeviceWidget *> found = ui->devicesWidget->findChildren<ColourfulDeviceWidget *>(deviceName);

    for (ColourfulDeviceWidget *dw : found) {
        dw->setColor(color);

        // 设置阴影效果
        QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
        effect->setOffset(0, 0);
        effect->setBlurRadius(15);
        effect->setColor(color.darker(130));
        dw->setGraphicsEffect(effect);
    }
}

// 一次更新所有设备的颜色 (内部会处理设备之间的渐变色)
void ColourfulDevicesWidget::updateDevices() {
    int len = devices.size();

    for (int i = 0; i < len; ++i) {
        QColor startColor = (i == 0) ?     devices[i]->getColor() : devices[i-1]->getColor();
        QColor endColor   = (i == len-1) ? devices[i]->getColor() : devices[i+1]->getColor();
        devices[i]->setGradientColors(startColor, endColor);
        devices[i]->update();
    }
}

// 放置设备
void ColourfulDevicesWidget::placeDevices() {
    QGridLayout *layout = qobject_cast<QGridLayout*>(ui->devicesWidget->layout());
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(0);

    int colCount = columnCountOfDevice;
    int rowCount = (devices.size() - 1) / colCount + 1;

    for (int row = 0; row < rowCount; ++row) {
        for (int col = 0; col < colCount; ++col) {
            int i = row * colCount + col;

            if (i < devices.size()) {
                layout->addWidget(devices.at(i), row, col);
            }
        }
    }
}

// 删除 widget 里面的所有子 widget，不会删除 layout
void ColourfulDevicesWidget::emptyWidget(QWidget *widget) {
    QLayout *layout = widget->layout();

    if (layout) {
        QLayoutItem *item;

        // The key point here is that the layout items are stored inside the layout in a stack
        while((item = layout->takeAt(0)) != nullptr) {
            if (item->widget()) {
                delete item->widget();
            } else {
                delete item;
            }
        }
    }
}
