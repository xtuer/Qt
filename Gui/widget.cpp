#include "widget.h"
#include "ui_widget.h"
#include "UiUtil.h"

#include <QDebug>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QThread>
#include <QDir>
#include <QFile>
#include <QPaintEvent>
#include <QPainterPath>
#include <QPainter>
#include <QStandardItemModel>
#include <QAction>
#include <QModelIndex>
#include <QToolTip>
#include <QPainter>
#include <QPixmap>
#include <QMatrix>
#include <QMatrix4x4>
#include <QVector3D>
#include <QtMath>
#include <QList>
#include <algorithm>
#include <QBrush>
#include <QColor>

struct Item {
    Item(const QString &imagePath, const QVector3D &center) : imagePath(imagePath), center(center) {}
    QString imagePath;
    QVector3D center;
};

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    loadQss();

    //    matrix.rotate(90, 0, 1, 0);

    //    QVector3D p(0, 0, 100);
    //    p = matrix.map(p);

    //    qDebug() << p;
    //    qDebug() << qCos(qDegreesToRadians(45.0));

    for (int i = 1; i <= 17; ++i) {
        items << new Item(QString("%1.jpg").arg(i), QVector3D());
    }

    QVector3D o(0, 0, 200); // carousel

    for (int i = 0; i < items.size(); ++i) {
        QMatrix4x4 matrix;
        matrix.rotate(360 / items.size() * i, 0, 1, -0.2);

        items[i]->center = matrix.map(o);
    }

    // 传入 sort 需要的比较函数，进行降序排序
    std::sort(items.begin(), items.end(), [](Item* a, Item* b) -> bool {
        return a->center.z() < b->center.z();
    });
}

Widget::~Widget() {
    delete ui;
}

void Widget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.translate(width()/2, height()/2);

    double lens = 800.0;

    foreach (const Item *item, items) {
        double rate = (item->center.z() + lens - 200) / lens;
        int w = 140 * rate;
        int h = 70 * rate;
        int cx = item->center.x();
        int cy = item->center.y();
        QRect rect(0, 0, w, h);
        rect.moveCenter(QPoint(cx, cy));

        painter.setBrush(QBrush(QColor(qrand() % 255, qrand() % 255, qrand() % 255)));
        painter.drawRect(rect);
        painter.drawPixmap(rect, QPixmap(item->imagePath));
    }
}

void Widget::loadQss() {
    QAction *action = new QAction();
    action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
    this->addAction(action);

    connect(action, &QAction::triggered, [] {
        UiUtil::loadQss();
    });
}




