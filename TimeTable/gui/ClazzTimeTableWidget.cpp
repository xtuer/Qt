#include "ClazzTimeTableWidget.h"
#include "ui_ClazzTimeTableWidget.h"
#include "gui/TimeTableItemWidget.h"
#include "bean/TimeTableData.h"
#include "bean/TimeTableItem.h"
#include "dao/TimeTableItemDao.h"
#include "gui/CourseTeacherDialog.h"
#include "util/DBUtil.h"
#include "util/UiUtil.h"
#include "util/ExportUtil.h"
#include "util/SettingsUtil.h"

#include <QDialog>
#include <QHBoxLayout>
#include <QItemSelectionModel>
#include <QEvent>
#include <QSqlTableModel>

ClazzTimeTableWidget::ClazzTimeTableWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClazzTimeTableWidget), timeTableData(NULL), courseTeacherDialog(NULL) {
    ui->setupUi(this);

    ui->splitter->setObjectName("splitter");
    SettingsUtil::restoreSplitterState(this->objectName(), ui->splitter);

    loadClazzes();
    readTimeTableData();

    connect(ui->tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(loadTimeTableItems(QModelIndex,QModelIndex)));
    connect(ui->courseTeacherButton, SIGNAL(clicked()), this, SLOT(showCourseTeacherDialog()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveTimeTableItems()));
    connect(ui->editableCheckBox, SIGNAL(toggled(bool)), this, SLOT(changeEditMode(bool)));

    connect(ui->exportTimeTableOfClazzButton, SIGNAL(clicked()), this, SLOT(exportTimeTableOfClazz()));
    connect(ui->exportAllTimeTableOfAllClazzesButton, SIGNAL(clicked()), this, SLOT(exportTimeTableOfAllClazzes()));

    ui->editableCheckBox->setChecked(false);
}

ClazzTimeTableWidget::~ClazzTimeTableWidget() {
    SettingsUtil::saveSplitterState(this->objectName(), ui->splitter);
    delete ui;
}

void ClazzTimeTableWidget::showEvent(QShowEvent*) {
    model->select();
    ui->tableView->setCurrentIndex(model->index(0, 1));
}

void ClazzTimeTableWidget::showCourseTeacherDialog() {
    if (NULL == courseTeacherDialog) {
        courseTeacherDialog = new CourseTeacherDialog(this);
    }

    courseTeacherDialog->show();
    courseTeacherDialog->raise();
}

void ClazzTimeTableWidget::loadTimeTableItems(const QModelIndex & current, const QModelIndex & previous) {
    Q_UNUSED(previous)

    if (!current.isValid()) { return; }

    // Get clazz id
    int clazzId = model->index(current.row(), 0).data().toInt();

    // Get time table items
    QList<TimeTableItem*> items;
    TimeTableItemDao::findByClazzId(clazzId, &items);

    // Show time table items
    UiUtil::setTimeTableItems(ui->timeTableWidget, items);
    qDeleteAll(items);
}

void ClazzTimeTableWidget::saveTimeTableItems() {
    // Get clazz id
    int row = ui->tableView->currentIndex().row();
    int clazzId = model->index(row, 0).data().toInt();

    // Get time table items
    QList<TimeTableItem*> items;
    UiUtil::getTimeTableItems(ui->timeTableWidget, &items);

    // Set item's clazz id
    foreach (TimeTableItem *item, items) {
        item->clazzId = clazzId;
    }

    TimeTableItemDao::updateClazzTimeTableItems(clazzId, items);

    qDeleteAll(items);
}

void ClazzTimeTableWidget::loadClazzes() {
    // 初始化 model
    model = new QSqlTableModel(this, DBUtil::getDatabase());
    model->setTable("clazz");
    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("班级"));
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->hideColumn(0);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void ClazzTimeTableWidget::readTimeTableData() {
    delete timeTableData;
    timeTableData = new TimeTableData(SettingsUtil::getTimeTableData());
    UiUtil::updateTimeTable(ui->timeTableWidget, *timeTableData, false, true);
}

void ClazzTimeTableWidget::changeEditMode(bool editable) {
    QObjectList children = ui->timeTableWidget->children();
    foreach (QObject *obj, children) {
        TimeTableItemWidget *ttiw = qobject_cast<TimeTableItemWidget*>(obj);
        if (ttiw != NULL) {
            ttiw->setEditable(editable);
        }
    }
}

void ClazzTimeTableWidget::exportTimeTableOfClazz() {
    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid()) { return; }

    int clazzId = model->index(index.row(), 0).data().toInt();

    if (clazzId > 0) {
        QList<int> clazzIds;
        clazzIds.append(clazzId);
        ExportUtil::saveTimeTable(*timeTableData, clazzIds, true);
    }
}

void ClazzTimeTableWidget::exportTimeTableOfAllClazzes() {
    int rowCount = model->rowCount();
    QList<int> clazzIds;

    for (int row = 0; row < rowCount; ++row) {
        clazzIds.append(model->index(row, 0).data().toInt());
    }

    if (clazzIds.size() > 0) {
        ExportUtil::saveTimeTable(*timeTableData, clazzIds, true);
    }
}
