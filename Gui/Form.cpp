#include "Form.h"
#include "ui_Form.h"

#include <QDebug>
#include <QImage>
#include <QMatrix>

Form::Form(QWidget *parent) : QWidget(parent), ui(new Ui::Form) {
    ui->setupUi(this);
}
