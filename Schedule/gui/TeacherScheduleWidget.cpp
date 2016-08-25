#include "TeacherScheduleWidget.h"
#include "ui_TeacherScheduleWidget.h"
#include "db/ConnectionPool.h"
#include "util/UiUtil.h"
#include "util/ConfigUtil.h"
#include "util/ScheduleExporter.h"
#include "bean/ScheduleItem.h"
#include "bean/ScheduleDescription.h"
#include "dao/ScheduleDao.h"
#include "gui/ScheduleItemWidget.h"

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

TeacherScheduleWidget::TeacherScheduleWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::TeacherScheduleWidget) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true); // 使得 QSS 生效
    currentTeacherId = 0;
    teacherModel = NULL;
    db = Singleton<ConnectionPool>::getInstance().openConnection();
    ui->scheduleWidget->setEnabled(false);

    handleEvents();
}

TeacherScheduleWidget::~TeacherScheduleWidget() {
    delete ui;
    Singleton<ConnectionPool>::getInstance().closeConnection(db);
}

void TeacherScheduleWidget::showEvent(QShowEvent *event) {
    Q_UNUSED(event);
    initializeTeacherModel();
}

void TeacherScheduleWidget::initializeTeacherModel() {
    // 班级的 classModel
    delete teacherModel;
    delete ui->teacherTableView->selectionModel();

    teacherModel = new QSqlTableModel(this, db);
    teacherModel->setTable("teacher");
    teacherModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    teacherModel->select();
    teacherModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    teacherModel->setHeaderData(1, Qt::Horizontal, tr("老师"));

    ui->teacherTableView->setModel(teacherModel);
    ui->teacherTableView->horizontalHeader()->setStretchLastSection(true);
    ui->teacherTableView->setAlternatingRowColors(true);
    ui->teacherTableView->hideColumn(0);
    ui->teacherTableView->hideColumn(2);
    ui->teacherTableView->verticalHeader()->hide();
    ui->teacherTableView->horizontalHeader()->hide();
    ui->teacherTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 选中不同的老师时，显示他的课程表
    connect(ui->teacherTableView->selectionModel(), &QItemSelectionModel::currentRowChanged, [this](const QModelIndex &current) {
        int row = current.row();
        int col = 0;

        // 当行变化时当前班级才变化
        currentTeacherId = teacherModel->data(teacherModel->index(row, col), Qt::DisplayRole).toInt();
        showSchedule(currentTeacherId);
    });

    ui->teacherTableView->setCurrentIndex(teacherModel->index(0, 1));
}

void TeacherScheduleWidget::showSchedule(int teacherId) {
    // 1. 加载课程表描述数据
    // 2. 加载课表
    // 3. 显示课表

    ScheduleDescription desc = ConfigUtil::readScheduleDescription();
    QList<ScheduleItem> scheduleItems = ScheduleDao::selectScheduleItemsByTeacherId(teacherId);
    UiUtil::showSchedule(ui->scheduleWidget, desc, scheduleItems, true, false, currentTeacherId);
}

void TeacherScheduleWidget::handleEvents() {
    // 导出当前老师的课表
    connect(ui->exportTeacherScheduleButton, &QPushButton::clicked, [this]() {
        ScheduleExporter::exportTeacherSchedule(currentTeacherId);
    });

    // 导出所有老师的课表
    connect(ui->exportAllTeacherScheduleButton, &QPushButton::clicked, [this]() {
        ScheduleExporter::exportAllTeacherSchedule();
    });
}
