#include "ArrangeDevicesWidget.h"
#include "ui_ArrangeDevicesWidget.h"
#include "AroundCirclesWidget.h"

#include <QDebug>
#include <QMenu>
#include <QPushButton>
#include <QRandomGenerator>

ArrangeDevicesWidget::ArrangeDevicesWidget(QWidget *parent) : QWidget(parent), ui(new Ui::ArrangeDevicesWidget) {
    ui->setupUi(this);

    // 随机生成颜色，可以是指定的
    for (int i = 0; i < 100; ++i) {
        int r = QRandomGenerator::global()->bounded(256);
        int g = QRandomGenerator::global()->bounded(256);
        int b = QRandomGenerator::global()->bounded(256);
        colors << QColor(r, g, b).name(); // 十六进制的颜色: #aabbed
    }

    circlesWidget = new AroundCirclesWidget(this);
    layout()->replaceWidget(ui->placeHolderWidget, circlesWidget);

    for (int i = 1; i <= 16; ++i) {
        addDevice(QString("Device-%1").arg(i), colors[i]);
    }

    this->setStyleSheet(QString(".DeviceButton {border: 1px solid gray;} .DeviceButton::menu-indicator {width: 0; height: 0;}"));
}

ArrangeDevicesWidget::~ArrangeDevicesWidget() {
    delete ui;
}

void ArrangeDevicesWidget::addDevice(const QString name, const QString color) {
    QPushButton *button = new QPushButton(name);

    button->setFlat(true);
    button->setProperty("class", "DeviceButton");
    button->setStyleSheet(QString("background: %1").arg(color));
    button->setProperty("color", color);
    button->setProperty("sn", 0);

    // 选择小圆序号的菜单
    QMenu *menu = new QMenu(this);
    menu->addAction("清除");
    for (int i = 1; i <= 38; ++i) {
        menu->addAction(QString::number(i));
    }
    button->setMenu(menu);

    connect(menu, &QMenu::triggered, [=](QAction *action) {
        int currSn = action->text().toInt();         // 当前选中的小圆的序号
        int prevSn = button->property("sn").toInt(); // 上次选中的小圆的序号

        // 清除前一次对应的小圆的背景色
        // 设置当前对应的小圆的背景色
        // 保存当前对应的小圆的序号 sn
        circlesWidget->setColor(prevSn, QColor());
        circlesWidget->setColor(currSn, QColor(color));
        button->setProperty("sn", currSn);
    });

    ui->devicesWidget->layout()->addWidget(button);
}
