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

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
//    ui->widget->setAttribute(Qt::WA_TransparentForMouseEvents,true);

    QGraphicsScene *scene = new QGraphicsScene();
    QFont font("微软雅黑");
    QString str = "中001过";
    scene->addText(str, font);
    ui->graphicsView->setScene(scene);
}

Widget::~Widget() {
    delete ui;
}
