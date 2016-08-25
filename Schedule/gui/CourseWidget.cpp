#include "CourseWidget.h"
#include "ui_CourseWidget.h"

#include "db/ConnectionPool.h"
#include "util/UiUtil.h"

#include <QSqlTableModel>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>

CourseWidget::CourseWidget(QWidget *parent) : QWidget(parent), ui(new Ui::CourseWidget) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true); // 使得 QSS 生效

    db = Singleton<ConnectionPool>::getInstance().openConnection();
    initializeModel();
    handleEvents();
}

CourseWidget::~CourseWidget() {
    delete ui;
    Singleton<ConnectionPool>::getInstance().closeConnection(db);
}

void CourseWidget::initializeModel() {
    // 初始化 model
    model = new QSqlTableModel(this, db);
    model->setTable("course");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("课程"));
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->hideColumn(0);
//    ui->tableView->verticalHeader()->hide();
    //UiUtil::setDefaultVerticalHeaderHeight(ui->tableView);
}

void CourseWidget::handleEvents() {
    // 添加
    connect(ui->insertButton, &QPushButton::clicked, [this]() {
        UiUtil::appendTableViewRow(this->ui->tableView, 1);
    });

    // 删除
    connect(ui->deleteButton, &QPushButton::clicked, [this]() {
        UiUtil::removeTableViewSelectedRow(this->ui->tableView);
    });

    // 取消
    connect(ui->cancelButton, &QPushButton::clicked, [this]() {
        this->model->revertAll();
        this->ui->tableView->setCurrentIndex(QModelIndex());
    });

    // 保存
    connect(ui->saveButton, &QPushButton::clicked, [this]() {
        if (this->model->submitAll()) {
        } else {
            qDebug() << this->model->lastError();
            QMessageBox::warning(this, tr("错误信息"), tr("课程名称不能重复或者不能为空"), QMessageBox::Ok);
        }
    });
}
