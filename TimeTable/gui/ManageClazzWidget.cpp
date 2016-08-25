#include "ManageClazzWidget.h"
#include "ui_ManageClazzWidget.h"
#include "util/DBUtil.h"
#include "util/UiUtil.h"

#include <QtSql>
#include <QModelIndex>
#include <QMessageBox>

ManageClazzWidget::ManageClazzWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::ManageClazzWidget) {
    ui->setupUi(this);

    // 初始化 model
    model = new QSqlTableModel(this, DBUtil::getDatabase());
    model->setTable("clazz");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("班级名称"));
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

ManageClazzWidget::~ManageClazzWidget() {
    delete ui;
}

void ManageClazzWidget::insert() {
    UiUtil::appendTableViewRow(ui->tableView, 1);
}

void ManageClazzWidget::remove() {
    UiUtil::removeTableViewSelectedRow(ui->tableView);
}

void ManageClazzWidget::cancel() {
    model->revertAll();
    ui->tableView->setCurrentIndex(QModelIndex());
}

void ManageClazzWidget::save() {
    if (model->submitAll()) {
        // emit clazzChanged();
    } else {
        qDebug() << model->lastError();
        QMessageBox::warning(this, tr("错误信息"), tr("注意: 班级名称不能重复"), QMessageBox::Ok);
    }
}
