#include "ClassScheduleWidget.h"
#include "ui_ClassScheduleWidget.h"

#include "db/ConnectionPool.h"
#include "util/UiUtil.h"
#include "util/ConfigUtil.h"
#include "util/Util.h"
#include "util/ScheduleExporter.h"
#include "bean/ScheduleItem.h"
#include "bean/ScheduleDescription.h"
#include "dao/ScheduleDao.h"
#include "gui/ScheduleItemWidget.h"
#include "gui/CourseTeacherDialog.h"

#include <QDebug>
#include <QShowEvent>
#include <QMessageBox>
#include <QComboBox>
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlRelation>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QModelIndex>
#include <QMap>
#include <QSqlRecord>
#include <QSqlField>
#include <QItemSelectionModel>
#include <QtGlobal>
#include <QDateTime>

ClassScheduleWidget::ClassScheduleWidget(QWidget *parent) : QWidget(parent), ui(new Ui::ClassScheduleWidget) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true); // 使得 QSS 生效
    currentClassId = 0;
    classModel = NULL;
    courseTeacherDialog = NULL;
    db = Singleton<ConnectionPool>::getInstance().openConnection();

    ui->saveButton->setEnabled(false);
    ui->resourceButton->setEnabled(false);
    ui->scheduleWidget->setEnabled(false);
    ui->randomScheduleButton->setEnabled(false);
    ui->loadArrangedScheduleButton->setEnabled(false);

    handleEvents();
}

ClassScheduleWidget::~ClassScheduleWidget() {
    delete ui;
    Singleton<ConnectionPool>::getInstance().closeConnection(db);
}

void ClassScheduleWidget::showEvent(QShowEvent *event) {
    Q_UNUSED(event);
    initializeClassModel();
}

void ClassScheduleWidget::initializeClassModel() {
    // 班级的 classModel
    delete classModel;
    delete ui->classTableView->selectionModel();

    classModel = new QSqlTableModel(this, db);
    classModel->setTable("class");
    classModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    classModel->select();
    classModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    classModel->setHeaderData(1, Qt::Horizontal, tr("班级"));

    ui->classTableView->setModel(classModel);
    ui->classTableView->horizontalHeader()->setStretchLastSection(true);
    ui->classTableView->setAlternatingRowColors(true);
    ui->classTableView->hideColumn(0);
    ui->classTableView->verticalHeader()->hide();
    ui->classTableView->horizontalHeader()->hide();
    ui->classTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 选中不同的班级时，显示它的课程表
    connect(ui->classTableView->selectionModel(), &QItemSelectionModel::currentRowChanged, [this](const QModelIndex &current) {
        int row = current.row();
        int col = 0;

        // 当行变化时当前班级才变化
        currentClassId = classModel->data(classModel->index(row, col), Qt::DisplayRole).toInt();
        showSchedule(currentClassId);
    });

    ui->classTableView->setCurrentIndex(classModel->index(0, 1));
}

void ClassScheduleWidget::handleEvents() {
    // 加载预先分配的课程，创建课程表
    connect(ui->loadArrangedScheduleButton, &QPushButton::clicked, [this]() {
        showArrangedSchedule(currentClassId);
    });

    // 保存课表到数据库
    connect(ui->saveButton, &QPushButton::clicked, [this]() {
        // 1. 取得当前班级的课表
        // 2. 删除数据库中当前班级的课表
        // 3. 插入当前班级的课表

        if (0 != currentClassId) {
            QList<ScheduleItem> items = getScheduleItems(true);
            ScheduleDao::deleteScheduleItemsByClassId(currentClassId);
            ScheduleDao::insertScheduleItems(items);
        }
    });

    // 随机排课
    connect(ui->randomScheduleButton, &QPushButton::clicked, [this]() {
        QList<ScheduleItem> items = getScheduleItems();
        shuffleSchedule(&items);
        ScheduleDescription desc = ConfigUtil::readScheduleDescription();
        UiUtil::showSchedule(ui->scheduleWidget, desc, items, false, true, currentClassId);
    });

    // 可否排课
    connect(ui->editableCheckBox, &QCheckBox::clicked, [this](bool checked) {
        ui->saveButton->setEnabled(checked);
        ui->resourceButton->setEnabled(checked);
        ui->scheduleWidget->setEnabled(checked);
        ui->randomScheduleButton->setEnabled(checked);
        ui->loadArrangedScheduleButton->setEnabled(checked);
    });

    // 打开老师，课程选择对话框
    connect(ui->resourceButton, &QPushButton::clicked, [this]() {
        // 只创建一个对话框的对象
        if (NULL == courseTeacherDialog) {
            courseTeacherDialog = new CourseTeacherDialog(this);
        }

        courseTeacherDialog->show();
        courseTeacherDialog->raise();
    });

    connect(ui->exportClassScheduleButton, &QPushButton::clicked, [this]() {
        ScheduleExporter::exportClassSchedule(currentClassId);
    });

    connect(ui->exportAllClassScheduleButton, &QPushButton::clicked, [this]() {
        ScheduleExporter::exportAllClassSchedule();
    });
}

void ClassScheduleWidget::showSchedule(int classId) {
    // 1. 加载课程表描述数据
    // 2. 加载课表
    // 3. 显示课表

    ScheduleDescription desc = ConfigUtil::readScheduleDescription();
    QList<ScheduleItem> scheduleItems = ScheduleDao::selectScheduleItemsByClassId(classId);
    UiUtil::showSchedule(ui->scheduleWidget, desc, scheduleItems, false, true, currentClassId);
}

void ClassScheduleWidget::showArrangedSchedule(int classId) {
    // 1. 加载课程表描述数据
    // 2. 查询班级预先分配的课程
    // 3. 按顺序分配课程: 给每个课程坐标 (day, time)
    // 4. 显示课程表

    ScheduleDescription desc = ConfigUtil::readScheduleDescription();
    QList<ScheduleItem> scheduleItems = ScheduleDao::selectArrangedScheduleItemByClassId(classId);

    // 从左到右，从上到下按顺序分配课程
    int i = 0;
    for (int time = 0; time <= desc.times; ++time) {
        for (int day = 0; day <= desc.days; ++day) {
            if (!desc.rests.contains(time) && time > 0 && day > 0 && i < scheduleItems.size()) {
                scheduleItems[i].day = day;
                scheduleItems[i].time = time;
                ++i;
            }
        }
    }

    UiUtil::showSchedule(ui->scheduleWidget, desc, scheduleItems, false, true, currentClassId);
}

// 从 scheduleWidget 中找到所有的 ScheduleItemWidget
QList<ScheduleItem> ClassScheduleWidget::getScheduleItems(bool valid) const {
    QList<ScheduleItem> items;
    QObjectList kids = ui->scheduleWidget->children();

    foreach (QObject *kid, kids) {
        ScheduleItemWidget *w = qobject_cast<ScheduleItemWidget*>(kid);

        if (NULL != w) {
            ScheduleItem item = w->getScheduleItem();

            if (valid) {
                if (0 != item.teacherId || 0 != item.courseId) {
                    items << item;
                }
            } else {
                items << item;
            }
        }
    }

    return items;
}

// 随机交换 2 个 schedule item 的 day 和 time
void ClassScheduleWidget::shuffleSchedule(QList<ScheduleItem> *items) {
    // 1. 如果 items 没有元素则返回
    // 2. 遍历 items, 生存 2 个随机数，范围为 [0, items->size() )
    // 3. 交换 2 个 schedule item 的 day 和 time

    if (items->size() == 0) { return; }

    qsrand(QDateTime::currentMSecsSinceEpoch()); // 随机数种子

    for (int i = 0; i < 100; ++i) {
        int index1 = qrand() % items->size();
        int index2 = qrand() % items->size();

        // 交换 day and time
        if (index1 != index2 && (*items)[index1].editable && (*items)[index2].editable) {
            Util::exchangeScheduleItemDayAndTime(&(*items)[index1], &(*items)[index2]);
        }
    }
}
