#include "Form.h"
#include "ui_Form.h"

#include <QDebug>

Form::Form(QWidget *parent) : QWidget(parent), ui(new Ui::Form) {
    ui->setupUi(this);
}
