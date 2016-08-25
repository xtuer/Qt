#include "ArrangeCourseWidget.h"
#include "ui_ArrangeCourseWidget.h"
#include "db/ConnectionPool.h"
#include "util/UiUtil.h"

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

ArrangeCourseWidget::ArrangeCourseWidget(QWidget *parent) : QWidget(parent), ui(new Ui::ArrangeCourseWidget) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true); // 使得 QSS 生效
    db = Singleton<ConnectionPool>::getInstance().openConnection();
//    initializeModel();
    arrangeModel = NULL;
    classModel = NULL;
    currentClassId = 0;

    handleEvents();
}

ArrangeCourseWidget::~ArrangeCourseWidget() {
    delete ui;
    Singleton<ConnectionPool>::getInstance().closeConnection(db);
}

void ArrangeCourseWidget::showEvent(QShowEvent *event) {
    Q_UNUSED(event);
    initializeClassModel();
}

void ArrangeCourseWidget::initializeClassModel() {
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
    ui->classTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(ui->classTableView->selectionModel(), &QItemSelectionModel::currentRowChanged, [this](const QModelIndex &current) {
        int row = current.row();
        int col = 0;
        currentClassId = classModel->data(classModel->index(row, col), Qt::DisplayRole).toInt();
        initializeArrangeModel(currentClassId);
    });

    ui->classTableView->setCurrentIndex(classModel->index(0, 1));
}

void ArrangeCourseWidget::initializeArrangeModel(int classId) {
    // 课程分配的 model
    delete arrangeModel;
    delete ui->tableView->selectionModel();

    arrangeModel = new QSqlRelationalTableModel(this, db);
    arrangeModel->setTable("arrange_course");
    arrangeModel->setFilter(QString("class_id=%1").arg(classId));
    arrangeModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    arrangeModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    arrangeModel->setHeaderData(1, Qt::Horizontal, tr("班级"));
    arrangeModel->setHeaderData(2, Qt::Horizontal, tr("课程"));
    arrangeModel->setHeaderData(3, Qt::Horizontal, tr("老师"));
    arrangeModel->setHeaderData(4, Qt::Horizontal, tr("课程数"));

    arrangeModel->setJoinMode(QSqlRelationalTableModel::LeftJoin);
    arrangeModel->setRelation(1, QSqlRelation("class", "id", "name"));
    arrangeModel->setRelation(2, QSqlRelation("course", "id", "name"));
    arrangeModel->setRelation(3, QSqlRelation("teacher", "id", "name"));
    arrangeModel->select();

    // item delegate
    ui->tableView->setModel(arrangeModel);
    ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));

    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->hideColumn(0); // 隐藏 ID 列
    ui->tableView->hideColumn(1); // 隐藏班级列
//    ui->tableView->verticalHeader()->hide();
}

void ArrangeCourseWidget::handleEvents() {
    // 添加
    connect(ui->insertButton, &QPushButton::clicked, [this]() {
        UiUtil::appendTableViewRow(this->ui->tableView, 2);

        // 得到选中的班级 id
//        int classRow = UiUtil::getTableViewSelectedIndex(this->ui->classTableView).row();
//        int classId = this->classModel->data(this->classModel->index(classRow, 0)).toInt();

        if (currentClassId == 0) {
            return;
        }

        // 设置最后一行的班级
        QModelIndex index = this->arrangeModel->index(this->arrangeModel->rowCount() - 1, 1);
        this->arrangeModel->setData(index, currentClassId);
    });

    // 删除
    connect(ui->deleteButton, &QPushButton::clicked, [this]() {
        UiUtil::removeTableViewSelectedRow(this->ui->tableView);
    });

    // 取消
    connect(ui->cancelButton, &QPushButton::clicked, [this]() {
        this->arrangeModel->revertAll();
        this->ui->tableView->setCurrentIndex(QModelIndex());
    });

    // 保存
    connect(ui->saveButton, &QPushButton::clicked, [this]() {
        if (this->arrangeModel->submitAll()) {
        } else {
            qDebug() << this->arrangeModel->lastError();
            QMessageBox::warning(this, tr("错误信息"), tr("班级名称不能重复或者不能为空"), QMessageBox::Ok);
        }
    });
}
