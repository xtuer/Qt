#include "LoginStatusWidget.h"
#include "ui_LoginStatusWidget.h"
#include "bean/Student.h"
#include <QStandardItem>
#include <QStandardItemModel>
#include <QDebug>
#include <QModelIndex>
#include <QToolTip>
#include <QClipboard>
#include <QApplication>

LoginStatusWidget::LoginStatusWidget(QWidget *parent) : QWidget(parent), ui(new Ui::LoginStatusWidget) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true); // 启用 QSS

    model = new QStandardItemModel();

    ui->listView->setModel(model);
    ui->listView->setWrapping(true); // 空间不够显示后自动折叠
    ui->listView->setFlow(QListView::LeftToRight); // 从左向右排列
    ui->listView->setViewMode(QListView::IconMode); // 文字在图标下面
    ui->listView->setResizeMode(QListView::Adjust); // 大小变化后重新布局 items
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers); // 不允许编辑
    ui->listView->setIconSize(QSize(36, 36)); // 图标大小
    ui->listView->setGridSize(QSize(55, 66));

    // 鼠标放到 item 上时显示提示
    ui->listView->setMouseTracking(true);
    connect(ui->listView, &QListView::entered, [] (const QModelIndex &index) {
        if (!index.isValid()) {
            qDebug() << "Invalid index";
            return;
        }

        QToolTip::showText(QCursor::pos(), index.data(Qt::UserRole + 1).toString());
    });

    // 双击复制准考证号
    connect(ui->listView, &QListView::clicked, [] (const QModelIndex &index) {
        if (!index.isValid()) {
            qDebug() << "Invalid index";
            return;
        }

        QString examName = index.data().toString();
        QString examUid = index.data(Qt::UserRole + 2).toString();
        QApplication::clipboard()->setText(examUid);
        QToolTip::showText(QCursor::pos(), QString("成功复制%1的准考证号: %2").arg(examName).arg(examUid));
    });
}

LoginStatusWidget::~LoginStatusWidget() {
    delete ui;
}

void LoginStatusWidget::setStudents(const QList<Student> &students) {
    model->clear();

    QIcon onlineIcon(":/icon/online.png");
    QIcon offlineIcon(":/icon/offline.png");

    // for (int i = 0; i < 2; ++i) // 测试滚动条
    foreach (const Student &student, students) {
        QStandardItem *item = new QStandardItem(student.signedAt.isEmpty() ? offlineIcon : onlineIcon, student.examineeName);
        QString toolTip = QString("姓名: %1<br>准考证号: %2<br>身份证号: %3<br>签到时间: %4")
                .arg(student.examineeName).arg(student.examUid).arg(student.idCardNo).arg(student.signedAt);
        item->setData(toolTip, Qt::UserRole + 1);
        item->setData(student.examUid, Qt::UserRole + 2); // 准考证号
        item->setSelectable(false);

        model->appendRow(item);
    }
}
