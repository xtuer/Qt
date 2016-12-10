#include "Widget.h"
#include "ui_Widget.h"
#include <QPainter>
#include <QPixmap>
#include <QStandardItem>
#include <QStandardItemModel>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    QStandardItemModel *model = new QStandardItemModel();

        for (int i = 0; i < 20; ++i) {
            QStandardItem *item = new QStandardItem(QString::number(i));
            model->appendRow(item);
        }

        ui->listView->setModel(model);
        ui->listView->setWrapping(true); // 空间不够显示后自动折叠
        ui->listView->setFlow(QListView::LeftToRight); // 从左向右排列
        ui->listView->setViewMode(QListView::IconMode); // 文字在图标下面
        ui->listView->setResizeMode(QListView::Adjust); // 大小变化后重新布局 items
        ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers); // 不允许编辑
        ui->listView->setIconSize(QSize(64, 64)); // 图标大小
        ui->listView->setGridSize(QSize(50, 50));
}

Widget::~Widget() {
    delete ui;
}

void Widget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
}
