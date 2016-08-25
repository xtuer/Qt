#include "DesignScheduleWidget.h"
#include "ui_DesignScheduleWidget.h"
#include "util/UiUtil.h"
#include "util/ConfigUtil.h"
#include "bean/ScheduleItem.h"
#include "bean/ScheduleDescription.h"
#include "Constants.h"

#include <QList>
#include <QLabel>
#include <QMouseEvent>
#include <QInputDialog>
#include <QFile>
#include <QDataStream>
#include <QDebug>

DesignScheduleWidget::DesignScheduleWidget(QWidget *parent) : QWidget(parent), ui(new Ui::DesignScheduleWidget) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true); // 使得 QSS 生效
    ui->scheduleWidget->installEventFilter(this);

    // 读取并显示课程表
    scheduleDescription = new ScheduleDescription(ConfigUtil::readScheduleDescription());
    ui->dayCountOfWeekSpinBox->setValue(scheduleDescription->dayCountOfWeek);
    ui->courseCountOfDaySpinBox->setValue(scheduleDescription->courseCountOfDay);
    showSchedule();

    // 事件处理
    handleEvents();
}

DesignScheduleWidget::~DesignScheduleWidget() {
    delete ui;
    delete scheduleDescription;
}

bool DesignScheduleWidget::eventFilter(QObject *watched, QEvent *event) {
    // 双击编辑星期和课程时间
    if (watched == ui->scheduleWidget && event->type() == QEvent::MouseButtonDblClick) {
        QMouseEvent *me = dynamic_cast<QMouseEvent*>(event);
        editDayOrTimeText(me->x(), me->y());
        return true;
    }

    return QWidget::eventFilter(watched, event);
}

void DesignScheduleWidget::handleEvents() {
    void (QSpinBox::*spinBoxValueChanged)(int) = &QSpinBox::valueChanged;

    // 修改每周上课的天数
    connect(ui->dayCountOfWeekSpinBox, spinBoxValueChanged, [this]() {
        this->scheduleDescription->dayCountOfWeek = this->ui->dayCountOfWeekSpinBox->value();
        showSchedule();
    });

    // 修改每天上课的课程数
    connect(ui->courseCountOfDaySpinBox, spinBoxValueChanged, [this]() {
        this->scheduleDescription->courseCountOfDay = this->ui->courseCountOfDaySpinBox->value();
        showSchedule();
    });

    // 保存课程表结构的数据
    connect(ui->saveButton, &QPushButton::clicked, [this]() {
        ConfigUtil::writeScheduleDescription(*this->scheduleDescription);
    });
}

void DesignScheduleWidget::editDayOrTimeText(int x, int y) {
    // 1. 选取编辑的 Label，第一行为星期，第一列为时间 (row > 0 或者 col > 0)
    // 2. 显示输入框，用户输入
    // 3. 保存编辑的数据到 scheduleDescription
    // 4. 使用 scheduleDescription 更新课程表

    // [1] 选取编辑的 Label，第一行为星期，第一列为时间 (row > 0 或者 col > 0)
    QLabel *label = qobject_cast<QLabel*>(ui->scheduleWidget->childAt(x, y));
    if (NULL == label) { return; }

    // 取得 label 的行和列
    int row = label->property(Constants::PROPERTY_ROW).toInt();
    int col = label->property(Constants::PROPERTY_COLUMN).toInt();

    if (row == 0 && col == 0) { return; } // 如果是左上角的 label，则不处理

    // [2] 显示输入框，用户输入
    QString type = (row > 0) ? "课程时间" : "星期";
    QString originalText = label->text();
    bool ok;
    QString text = QInputDialog::getText(this, tr(""), tr("请输入%1:").arg(type), QLineEdit::Normal, originalText, &ok);
    if (!ok || text.isEmpty()) { return; } // 取消输入或者输入空字符串则返回

    // [3] 保存编辑的数据到 scheduleDescription
    // col > 0 则修改星期, row > 0 则修改时间
    if (col > 0) {
        scheduleDescription->setDayText(col, text);
    } else {
        scheduleDescription->setTimeText(row, text);
    }

    // [4] 使用 scheduleDescription 更新课程表
    showSchedule();
}

void DesignScheduleWidget::showSchedule() {
    UiUtil::showSchedule(ui->scheduleWidget, *scheduleDescription, QList<ScheduleItem>(), false, false, 0);
}
