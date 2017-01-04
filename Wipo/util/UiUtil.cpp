#include "UiUtil.h"
#include "util/Config.h"

#include <QFile>
#include <QStringList>
#include <QDebug>
#include <QApplication>

#include <QWidget>
#include <QStackedWidget>
#include <QSpacerItem>
#include <QGridLayout>

void UiUtil::loadQss() {
    QStringList qssFileNames(Config::getQssFilePath());
    QString qss;

    foreach (QString name, qssFileNames) {
        qDebug() << QString("加载 QSS 文件 %1").arg(name);

        QFile file(name);
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "Cannot open file: " << name;
            continue;
        }

        qss.append(file.readAll()).append("\n");
        file.close();
    }

    if (!qss.isEmpty()) {
        qApp->setStyleSheet(qss);
    }
}

void UiUtil::updateQss(QWidget *widget) {
    widget->setStyleSheet("/**/");
}

/**
 * 把 widget 加入到 stacked widget 里, 可以设置向四个方向的伸展
 */
void UiUtil::addWidgetIntoStackedWidget(QWidget *widget, QStackedWidget *stackedWidget,
                                        bool toLeft,
                                        bool toTop,
                                        bool toRight,
                                        bool toBottom) {
    // 使用 widget 居左上
    QGridLayout *layout = new QGridLayout();

    QSpacerItem *spacer = NULL;
    if (!toLeft) {
        spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
        layout->addItem(spacer, 1, 0);
    }

    if (!toTop) {
        spacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
        layout->addItem(spacer, 0, 1);
    }

    if (!toRight) {
        spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
        layout->addItem(spacer, 1, 2);
    }

    if (!toBottom) {
        spacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
        layout->addItem(spacer, 2, 1);
    }

    QWidget *container = new QWidget();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(widget, 1, 1);
    container->setLayout(layout);
    stackedWidget->addWidget(container);
}
