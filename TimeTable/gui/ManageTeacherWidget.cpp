#include "ManageTeacherWidget.h"
#include "ui_ManageTeacherWidget.h"

#include "bean/TimeTableItem.h"
#include "bean/TimeTableData.h"
#include "dao/TimeTableItemDao.h"

#include "util/DBUtil.h"
#include "util/UiUtil.h"
#include "util/SettingsUtil.h"

#include <QtSql>
#include <QModelIndex>
#include <QMessageBox>

ManageTeacherWidget::ManageTeacherWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::ManageTeacherWidget) {
    ui->setupUi(this);

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

    connect(ui->insertButton, SIGNAL(clicked()), this, SLOT(insert()));
    connect(ui->removeButton, SIGNAL(clicked()), this, SLOT(remove()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(save()));
}

ManageTeacherWidget::~ManageTeacherWidget() {
    delete ui;
}

void ManageTeacherWidget::insert() {
    UiUtil::appendTableViewRow(ui->tableView, 1);
}

void ManageTeacherWidget::remove() {
    UiUtil::removeTableViewSelectedRow(ui->tableView);
}

void ManageTeacherWidget::cancel() {
    model->revertAll();
    ui->tableView->setCurrentIndex(QModelIndex());
}

void ManageTeacherWidget::save() {
    if (model->submitAll()) {
        // emit clazzChanged();
    } else {
        qDebug() << model->lastError();
        QMessageBox::warning(this, tr("错误信息"), tr("注意: 教师名称不能重复"), QMessageBox::Ok);
    }
}
