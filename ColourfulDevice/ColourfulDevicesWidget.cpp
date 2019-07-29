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

void ColourfulDevicesWidget::createDevices(QList<QString> deviceNames) {
    devices.clear();
    emptyWidget(ui->devicesWidget);

    for (const QString &name : deviceNames) {
        devices.append(new ColourfulDeviceWidget(name));
    }

    // 放置设备
    placeDevices();
}

void ColourfulDevicesWidget::setDeviceColor(const QString &deviceName, const QColor &color) {
    QList<ColourfulDeviceWidget *> found = ui->devicesWidget->findChildren<ColourfulDeviceWidget *>(deviceName);

    for (ColourfulDeviceWidget *dw : found) {
        dw->setColor(color);
    }
}

void ColourfulDevicesWidget::updateDevices() {
    int len = devices.size();

    for (int i = 0; i < len; ++i) {
        QColor startColor = i == 0 ? Qt::transparent : devices[i-1]->getColor();
        QColor endColor   = (i == len-1) ? Qt::transparent : devices[i+1]->getColor();
        devices[i]->setGradientColors(startColor, endColor);
        devices[i]->update();
    }
}

// 放置设备
void ColourfulDevicesWidget::placeDevices() {
    QGridLayout *layout = qobject_cast<QGridLayout*>(ui->devicesWidget->layout());
    layout->setContentsMargins(0, 0, 0, 0);
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

void ColourfulDevicesWidget::initialize() {
    ui->setupUi(this);
    this->layout()->setSizeConstraint(QLayout::SetFixedSize);
}

void ColourfulDevicesWidget::handleEvents() {

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
