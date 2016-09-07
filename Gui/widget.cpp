#include "widget.h"
#include "ui_widget.h"
#include "OxyWidget.h"

#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QTextStream>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QGraphicsScene>
#include <QFont>
#include <QSplitter>
#include <QResizeEvent>
#include <QRubberBand>
#include <QDateTime>
#include <QPixmap>
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QApplication>
#include <QWidgetList>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
//    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_StyledBackground, true);
    setObjectName("Widget");

    connect(ui->pushButton, &QPushButton::clicked, [] {
        foreach (QWidget *w, QApplication::topLevelWidgets()) {
            qDebug() << w->objectName();
        }
    });
}

Widget::~Widget() {
    delete ui;
}

void Widget::resizeEvent(QResizeEvent *event) {

}

void Widget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(Qt::gray));
    painter.setPen(Qt::gray);
//    painter.drawRoundedRect(0, 0, width(), height(), 50, 50);
}
