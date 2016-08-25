#include "ManageCourseWidget.h"
#include "ui_ManageCourseWidget.h"

#include "util/DBUtil.h"
#include "util/UiUtil.h"

#include <QtSql>
#include <QModelIndex>
#include <QMessageBox>

ManageCourseWidget::ManageCourseWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::ManageCourseWidget) {
    ui->setupUi(this);

    // 初始化 model
    model = new QSqlTableModel(this, DBUtil::getDatabase());
    model->setTable("course");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("课程名称"));
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setAlternatingRowColors(true);
    //UiUtil::setDefaultVerticalHeaderHeight(ui->tableView);
    ui->tableView->hideColumn(0);
    ui->tableView->verticalHeader()->hide();

    connect(ui->insertButton, SIGNAL(clicked()), this, SLOT(insert()));
    connect(ui->removeButton, SIGNAL(clicked()), this, SLOT(remove()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(save()));
}

ManageCourseWidget::~ManageCourseWidget() {
    delete ui;
}

void ManageCourseWidget::insert() {
    UiUtil::appendTableViewRow(ui->tableView, 1);
}

void ManageCourseWidget::remove() {
    UiUtil::removeTableViewSelectedRow(ui->tableView);
}

void ManageCourseWidget::cancel() {
    model->revertAll();
    ui->tableView->setCurrentIndex(QModelIndex());
}

void ManageCourseWidget::save() {
    if (model->submitAll()) {
        // emit clazzChanged();
    } else {
        qDebug() << model->lastError();
        QMessageBox::warning(this, tr("错误信息"), tr("注意: 课程名称不能重复"), QMessageBox::Ok);
    }
}
