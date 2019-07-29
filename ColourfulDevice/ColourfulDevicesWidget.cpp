#include "ColourfulDeviceWidget.h"
#include "ui_ColourfulDeviceWidget.h"
#include "GradientColorPicker.h"

#include <QDebug>
#include <QPalette>
#include <QRandomGenerator>

ColourfulDeviceWidget::ColourfulDeviceWidget(QWidget *parent) : QWidget(parent), ui(new Ui::ColourfulDeviceWidget) {
    initialize();
    handleEvents();
    ui->comboBox->setCurrentIndex(0);
}

ColourfulDeviceWidget::~ColourfulDeviceWidget() {
    delete ui;
    delete colorPicker;
}

void ColourfulDeviceWidget::initialize() {
    ui->setupUi(this);
    ui->comboBox->setCurrentIndex(-1);
    this->layout()->setSizeConstraint(QLayout::SetFixedSize);

    colorPicker = new GradientColorPicker();
}

void ColourfulDeviceWidget::handleEvents() {
    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index) {
        emptyWidget(ui->devicesWidget);

        int rowCount = index == 0 ? 2 : 3;
        int colCount = 6;

        QGridLayout *layout = new QGridLayout();
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);

        for (int row = 0; row < rowCount; ++row) {
            for (int col = 0; col < colCount; ++col) {
                // TODO: 每个 label 对应一个设备，设备的数量和它的温度作为参数输入进来
                // 获取温度的比例: (temperature-minTemperature)/(maxTemperature-minTemperature)
                double temperature = QRandomGenerator::global()->bounded(200.0);
                double temperatureFactor = (temperature - 0) / (200.0 - 0.0);
                QLabel *label = new QLabel(QString("%1-%2<br>%3℃").arg(row+1).arg(col+1).arg(int(temperature)));

                // 每个比例值对应的颜色
                QColor color = colorPicker->getColor(temperatureFactor);

                // 设置背景色
                QPalette palette = label->palette();
                palette.setBrush(QPalette::Window, color);
                label->setPalette(palette);
                label->setAutoFillBackground(true);

                label->setAlignment(Qt::AlignCenter);
                label->setMinimumSize(60, 60);
                label->setMaximumSize(60, 60);
                layout->addWidget(label, row, col);
            }
        }

        layout->setRowStretch(rowCount, 1);
        ui->devicesWidget->setLayout(layout);
    });
}

// 删除 widget 里面的所有子 widget 和 layout
void ColourfulDeviceWidget::emptyWidget(QWidget *widget) {
    QLayout *layout = widget->layout();

    if (layout) {
        QLayoutItem *item;

        // The key point here is that the layout items are stored inside the layout in a stack
        while((item = layout->takeAt(0)) != nullptr) {
            if (item->widget()) {
                layout->removeWidget(item->widget());
                delete item->widget();
            }

            delete item;
        }

        delete layout;
    }
}
