#include "TeacherTimeTableWidget.h"
#include "ui_TeacherTimeTableWidget.h"
#include "gui/TimeTableItemWidget.h"

#include "bean/TimeTableData.h"
#include "bean/TimeTableItem.h"
#include "dao/TimeTableItemDao.h"
#include "util/UiUtil.h"
#include "util/DBUtil.h"
#include "util/ExportUtil.h"
#include "util/SettingsUtil.h"

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>

TeacherTimeTableWidget::TeacherTimeTableWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TeacherTimeTableWidget), timeTableData(NULL) {
    ui->setupUi(this);

    readTimeTableData();

    // 初始化 model
    model = new QSqlTableModel(this, DBUtil::getDatabase());
    model->setTable("teacher");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("教师名称"));
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(2);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(ui->tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(loadTimeTableItems(QModelIndex,QModelIndex)));
    connect(ui->exportTimeTableOfTeacherButton, SIGNAL(clicked()), this, SLOT(exportTimeTableOfTeacher()));
    connect(ui->exportTimeTableOfAllTeachersButton, SIGNAL(clicked()), this, SLOT(exportTimeTableOfAllTeachers()));

    ui->splitter->setObjectName("splitter");
    SettingsUtil::restoreSplitterState(this->objectName(), ui->splitter);
}

TeacherTimeTableWidget::~TeacherTimeTableWidget() {
    SettingsUtil::saveSplitterState(this->objectName(), ui->splitter);
    delete ui;
}

void TeacherTimeTableWidget::showEvent(QShowEvent *event) {
    Q_UNUSED(event)
    model->select();
    ui->tableView->setCurrentIndex(model->index(0, 1));
}

void TeacherTimeTableWidget::readTimeTableData() {
    delete timeTableData;

    timeTableData = new TimeTableData(SettingsUtil::getTimeTableData());
    UiUtil::updateTimeTable(ui->timeTableWidget, *timeTableData, true, false);

    QObjectList children = ui->timeTableWidget->children();
    foreach (QObject *obj, children) {
        TimeTableItemWidget *ttiw = qobject_cast<TimeTableItemWidget*>(obj);
        if (ttiw != NULL) {
            ttiw->setEditable(false);
        }
    }
}

void TeacherTimeTableWidget::loadTimeTableItems(const QModelIndex &current, const QModelIndex &previous) {
    Q_UNUSED(previous)

    if (!current.isValid()) { return; }

    // Get clazz id
    int teacherId = model->index(current.row(), 0).data().toInt();

    // Get time table items
    QList<TimeTableItem*> items;
    TimeTableItemDao::findByTeacherId(teacherId, &items);

    // Show time table items
    UiUtil::setTimeTableItems(ui->timeTableWidget, items);
    qDeleteAll(items);
}

void TeacherTimeTableWidget::exportTimeTableOfTeacher() {
    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid()) { return; }

    int teacherId = model->index(index.row(), 0).data().toInt();

    if (teacherId > 0) {
        QList<int> teacherIds;
        teacherIds.append(teacherId);
        ExportUtil::saveTimeTable(*timeTableData, teacherIds, false);
    }
}

void TeacherTimeTableWidget::exportTimeTableOfAllTeachers() {
    int rowCount = model->rowCount();
    QList<int> teacherIds;

    for (int row = 0; row < rowCount; ++row) {
        teacherIds.append(model->index(row, 0).data().toInt());
    }

    if (teacherIds.size() > 0) {
        ExportUtil::saveTimeTable(*timeTableData, teacherIds, false);
    }
}
