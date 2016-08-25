#include "DesignTimeTableWidget.h"
#include "ui_DesignTimeTableWidget.h"

#include "gui/CourseTeacherWidget.h"
#include "bean/TimeTableData.h"
#include "Constants.h"
#include "util/UiUtil.h"
#include "util/SettingsUtil.h"

#include <QButtonGroup>
#include <QLayout>
#include <QLabel>
#include <QGridLayout>
#include <QDebug>
#include <QMouseEvent>
#include <QInputDialog>

DesignTimeTableWidget::DesignTimeTableWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::DesignTimeTableWidget) {
    ui->setupUi(this);

    timeTableData = new TimeTableData();
    ui->timeTableWidget->installEventFilter(this);
    readTimeTableData();

    connect(ui->buttonGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(updateTimeTableData(QAbstractButton*)));
    connect(ui->readButton, SIGNAL(clicked()), this, SLOT(readTimeTableData()));
    connect(ui->writeButton, SIGNAL(clicked()), this, SLOT(writeTimeTableData()));
}

DesignTimeTableWidget::~DesignTimeTableWidget() {
    delete ui;
    delete timeTableData;
}

bool DesignTimeTableWidget::eventFilter(QObject* watched, QEvent* event) {
    if (watched == ui->timeTableWidget && event->type() == QEvent::MouseButtonDblClick) {
        QMouseEvent *me = dynamic_cast<QMouseEvent*>(event);
        editTitle(me->x(), me->y());
        return true;
    }

    return QWidget::eventFilter(watched, event);
}

/**
 * @brief 修改课程表的数据: 行, 列, 休息时间
 * @param button
 */
void DesignTimeTableWidget::updateTimeTableData(QAbstractButton *button) {
    if (button == ui->addColumnButton) {
        timeTableData->setCol(timeTableData->getCol() + 1);
    } else if (button == ui->appendRestButton) {
        timeTableData->appendRest();
    } else if (button == ui->addRowButton) {
        timeTableData->setRow(timeTableData->getRow() + 1);
    } else if (button == ui->reduceColumnButton) {
        timeTableData->setCol(timeTableData->getCol() - 1);
    } else if (button == ui->removeRestButton) {
        timeTableData->removeRest();
    } else if (button == ui->reduceRowButton) {
        timeTableData->setRow(timeTableData->getRow() - 1);
    }

    UiUtil::updateTimeTable(ui->timeTableWidget, *timeTableData);
}

void DesignTimeTableWidget::readTimeTableData() {
    delete timeTableData;
    timeTableData = new TimeTableData(SettingsUtil::getTimeTableData());
    UiUtil::updateTimeTable(ui->timeTableWidget, *timeTableData);
}

void DesignTimeTableWidget::writeTimeTableData() {
    SettingsUtil::saveTimeTableData(*timeTableData);
}

void DesignTimeTableWidget::editTitle(int x, int y) {
    // 编辑上边的星期, 左边的时间文本
    QLabel *child = qobject_cast<QLabel*>(ui->timeTableWidget->childAt(x, y));
    if (NULL == child) { return; }

    // Get widget's position and text in the grid layout
    QString oriText = child->text();
    int row = child->property(Constants::PROPERTY_ROW).toInt();
    int col = child->property(Constants::PROPERTY_COLUMN).toInt();
    int span = child->property(Constants::PROPERTY_COLUMN_SPAN).toInt();
    int restIndex = child->property(Constants::PROPERTY_LAYOUT_REST_INDEX).toInt();

    // 非第一行, 或第一列, 且不是(0, 0), 返回
    if ((row == 0 && col == 0) || (row != 0 && col != 0)) { return; }

    bool ok;
    QString text = QInputDialog::getText(this, tr(""), tr("请输入:"), QLineEdit::Normal, oriText, &ok);

    if (!ok || text.isEmpty()) { return; }

    if (row == 0) {
        timeTableData->setDayText(col, text);
    } else if (col == 0 && span != 0) {
        timeTableData->setRestText(restIndex, text);
    } else if (col == 0) {
        timeTableData->setTimeText(row, text);
    }

    // 显示更新效果
    UiUtil::updateTimeTable(ui->timeTableWidget, *timeTableData);
}
