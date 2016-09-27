#include "UiUtil.h"
#include "util/Util.h"
#include "util/ConfigUtil.h"

#include <QFile>
#include <QStringList>
#include <QDebug>
#include <QApplication>

#include <QLabel>
#include <QWidget>
#include <QStackedWidget>
#include <QSpacerItem>
#include <QGridLayout>
#include <QTableView>
#include <QItemSelectionModel>
#include <QModelIndexList>
#include <QModelIndex>

void UiUtil::loadQss() {
    QStringList qssFileNames = Singleton<ConfigUtil>::getInstance().getQssFiles();
    QString qss;

    foreach (QString name, qssFileNames) {
        qDebug() << QString("=> 加载 QSS 文件 %1").arg(name);

        QFile file(name);
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << QString("打开 QSS 文件 %1 失败");
            continue;
        }

        qss.append(file.readAll()).append("\n");
        file.close();
    }

    if (!qss.isEmpty()) {
        //        qDebug() << qss;
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

void UiUtil::setWidgetPaddingAndSpacing(QWidget *widget, int padding, int spacing) {
    // 设置 Widget 的 padding 和 spacing
    QLayout *layout = widget->layout();

    if (NULL != layout) {
        layout->setContentsMargins(padding, padding, padding, padding);
        layout->setSpacing(spacing);
    }
}

QModelIndex UiUtil::getTableViewSelectedIndex(QTableView *view) {
    QItemSelectionModel *ism = view->selectionModel();
    QModelIndexList mil = ism->selectedIndexes();

    return mil.count() > 0 ? mil.at(0) : QModelIndex();
}

void UiUtil::appendTableViewRow(QTableView *view, int editColumn) {
    QAbstractItemModel *model = view->model();
    int row = model->rowCount();
    model->insertRow(row);

    QModelIndex index = model->index(row, editColumn);
    if (!index.isValid()) { return; }

    view->setCurrentIndex(index);
    view->edit(index);
}

void UiUtil::removeTableViewSelectedRow(QTableView *view) {
    QModelIndex index = getTableViewSelectedIndex(view);

    if (index.isValid()) {
        view->model()->removeRow(index.row());
    }
}

void UiUtil::deleteChildrenAndLayout(QWidget *w) {
    QLayout *layout = w->layout();

    if (layout != NULL) {
        QLayoutItem *child;

        while ((child = layout->itemAt(0)) != NULL) {
            delete child->widget();
        }

        delete layout;
    }
}
