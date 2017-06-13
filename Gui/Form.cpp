#include "Form.h"
#include "ui_Form.h"
#include <QDebug>
#include <QApplication>

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form) {
    ui->setupUi(this);
    setAcceptDrops(true);
}

Form::~Form() {
    delete ui;
}

void Form::dragEnterEvent(QDragEnterEvent *) {
    qDebug() << "Enter";
    QApplication::setOverrideCursor(Qt::WaitCursor);
}

void Form::enterEvent(QEvent *event)
{
    qDebug() << "---";
}
