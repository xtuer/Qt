#include "Form.h"
#include "ui_Form.h"

#include <QDebug>
#include <QImage>
#include <QMatrix>
#include <QPixmap>
#include <QPainter>
#include <QList>
#include <QStandardItem>

Form::Form(QWidget *parent) : QWidget(parent), ui(new Ui::Form) {
    ui->setupUi(this);
}
