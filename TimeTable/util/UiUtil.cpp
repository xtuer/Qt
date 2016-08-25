#include "UiUtil.h"
#include "Constants.h"
#include "bean/TimeTableData.h"
#include "bean/TimeTableItem.h"
#include "gui/TimeTableItemWidget.h"
#include "util/SettingsUtil.h"

#include <QWidget>
#include <QLabel>
#include <QList>
#include <QDebug>
#include <QObjectList>
#include <QString>
#include <QVariant>
#include <QGridLayout>
#include <QFile>
#include <QApplication>
#include <QTableView>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QStackedWidget>
#include <QGridLayout>
#include <QSpacerItem>

/**
 * 加载程序的 style sheet
 */
void UiUtil::loadStyleSheet() {
    QFile qss(SettingsUtil::getStyleSheetFileName());

    if (qss.exists()) {
        qss.open(QFile::ReadOnly);
        qApp->setStyleSheet(qss.readAll());
    }

    qss.close();
}

void UiUtil::updateStyleSheet(QWidget* widget) {
    widget->setStyleSheet("/**/");
}

QModelIndex UiUtil::getTableViewSelectedIndex(QTableView *view) {
    QItemSelectionModel *ism = view->selectionModel();
    QModelIndexList mil = ism->selectedIndexes();

    return mil.count() > 0 ? mil.at(0) : QModelIndex();
}

void UiUtil::removeTableViewSelectedRow(QTableView *view) {
    QModelIndex index = getTableViewSelectedIndex(view);

    if (index.isValid()) {
        view->model()->removeRow(index.row());
    }
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

/**
 * 显示课程表
 */
void UiUtil::updateTimeTable(QWidget *timeTableWidget, const TimeTableData &timeTableData,
                             bool showClazz,
                             bool showTeacher) {
    // 1. 删除表中已有的widgets.
    QLayout *layout = timeTableWidget->layout();

    if (layout != NULL) {
        QLayoutItem *child;

        while ((child = layout->itemAt(0)) != NULL) {
            delete child->widget();
        }

        delete layout;
    }

    // Set padding and spacing.
    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setContentsMargins(1, 1, 1, 1);
    gridLayout->setSpacing(1);

    int courseRow = 0;
    int rowCount = timeTableData.getRow();
    int colCount = timeTableData.getCol();
    int restIndex = 0;

    for (int row = 0; row < rowCount; ++row) {
        // 添加休息条,占一行
        if (timeTableData.isRest(row)) {
            QLabel *label = new QLabel("");
            label->setText(timeTableData.getRestText(restIndex));
            label->setProperty(Constants::QSS_CLASS, Constants::QSS_CLASS_REST);
            label->setProperty(Constants::PROPERTY_ROW, row);
            label->setProperty(Constants::PROPERTY_COLUMN, 0);
            label->setProperty(Constants::PROPERTY_COLUMN_SPAN, colCount);
            label->setProperty(Constants::PROPERTY_LAYOUT_REST_INDEX, restIndex);

            label->setAlignment(Qt::AlignCenter);
            gridLayout->addWidget(label, row, 0, 1, colCount);

            ++restIndex;

            continue;
        }

        if (row != 0) {
            ++courseRow; // 如果不是第一行且不是休息行,则这一行表示课程行
        }

        for (int col = 0; col < colCount; ++col) {
            // 第一行或第一列
            if (row == 0 || col == 0) {
                QLabel *label = new QLabel();
                label->setProperty(Constants::PROPERTY_ROW, row); // 存储在 layout 中的位置
                label->setProperty(Constants::PROPERTY_COLUMN, col);
                label->setAlignment(Qt::AlignCenter);

                if (row == 0) {
                    // 第一行是星期的标题
                    label->setProperty(Constants::QSS_CLASS, Constants::QSS_CLASS_DAY_TEXT);
                    label->setText(timeTableData.getDayText(col));
                } else {
                    // 第一列是时间的标题
                    label->setProperty(Constants::QSS_CLASS, Constants::QSS_CLASS_TIME_TEXT);
                    label->setText(timeTableData.getTimeText(row));
                }

                gridLayout->addWidget(label, row, col);
            }

            // 非第一行第一列表示课程
            if (row!=0 && col!=0) {
                TimeTableItemWidget *itemWidget = new TimeTableItemWidget();

                itemWidget->setProperty(Constants::PROPERTY_ROW, row); // 存储在 layout 中的位置
                itemWidget->setProperty(Constants::PROPERTY_COLUMN, col);

                itemWidget->showClazz(showClazz);
                itemWidget->showTeacher(showTeacher);

                TimeTableItem item;
                item.row = courseRow;
                item.col = col;
                itemWidget->setTimeTableItem(item);

                gridLayout->addWidget(itemWidget, row, col);
            }
        }
    }

    timeTableWidget->setLayout(gridLayout);
}

void UiUtil::setTimeTableItems(QWidget* timeTableWidget, const QList<TimeTableItem*> &items) {
    QObjectList children = timeTableWidget->children();
    TimeTableItem defaultItem;

    foreach (QObject *child, children) {
        TimeTableItemWidget *ttiw = qobject_cast<TimeTableItemWidget*>(child);

        if (ttiw != NULL) {
            ttiw->setTimeTableItem(defaultItem); // Clear the item.
            int row = ttiw->property(Constants::PROPERTY_ROW).toInt();
            int col = ttiw->property(Constants::PROPERTY_COLUMN).toInt();

            foreach (TimeTableItem *item, items) {
                if (item->row == row && item->col == col) {
                    ttiw->setTimeTableItem(*item);
                    break;
                }
            }
        }
    }
}

void UiUtil::getTimeTableItems(QWidget* timeTableWidget, QList<TimeTableItem*> *items) {
    qDeleteAll(*items);
    items->clear();

    QObjectList children = timeTableWidget->children();

    foreach (QObject *child, children) {
        TimeTableItemWidget *ttiw = qobject_cast<TimeTableItemWidget*>(child);

        if (ttiw != NULL) {
            items->append(new TimeTableItem(ttiw->getTimeTableItem()));
        }
    }
}
