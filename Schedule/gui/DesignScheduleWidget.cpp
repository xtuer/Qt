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
    connect(ui->infoEdit, &QLineEdit::textChanged, [this]() {
        this->scheduleDescription->info = this->ui->infoEdit->text();
    });
    connect(ui->increaseDayButton, &QPushButton::clicked, [this]() {
        this->scheduleDescription->increaseDay();
        this->showSchedule();
    });

    connect(ui->decreaseDayButton, &QPushButton::clicked, [this]() {
        this->scheduleDescription->decreaseDay();
        this->showSchedule();
    });

    connect(ui->increaseTimeButton, &QPushButton::clicked, [this]() {
        this->scheduleDescription->increaseTime();
        this->showSchedule();
    });

    connect(ui->decreaseTimeButton, &QPushButton::clicked, [this]() {
        this->scheduleDescription->decreaseTime();
        this->showSchedule();
    });

    connect(ui->increaseRestButton, &QPushButton::clicked, [this]() {
        this->scheduleDescription->increaseRest();
        this->showSchedule();
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
    QString className = label->property(Constants::QSS_CLASS).toString();
    int row = label->property(Constants::PROPERTY_ROW).toInt();
    int col = label->property(Constants::PROPERTY_COLUMN).toInt();

    if (row == 0 && col == 0) { return; } // 如果是左上角的 label，则不处理

    bool ok;
    QString text = QInputDialog::getText(this, "", "", QLineEdit::Normal, label->text(), &ok);
    if (!ok || text.isEmpty()) { return; } // 取消输入或者输入空字符串则返回

    if (Constants::QSS_CLASS_DAY_LABEL == className) {
        // 星期
        scheduleDescription->setDayText(col, text);
    } else if (Constants::QSS_CLASS_TIME_LABEL == className) {
        // 时间
        scheduleDescription->setTimeText(row, text);
    } else if (Constants::QSS_CLASS_REST_LABEL == className) {
        // 休息
        scheduleDescription->setRestText(row, text);
    }

    // [4] 使用 scheduleDescription 更新课程表
    showSchedule();
}

void DesignScheduleWidget::showSchedule() {
    ui->infoEdit->setText(scheduleDescription->info);
    UiUtil::showSchedule(ui->scheduleWidget, *scheduleDescription, QList<ScheduleItem>(), false, false, 0);
}
