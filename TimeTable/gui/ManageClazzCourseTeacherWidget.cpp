#include "ManageClazzCourseTeacherWidget.h"
#include "ui_ManageClazzCourseTeacherWidget.h"

ManageClazzCourseTeacherWidget::ManageClazzCourseTeacherWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ManageClazzCourseTeacherWidget) {
    ui->setupUi(this);
}

ManageClazzCourseTeacherWidget::~ManageClazzCourseTeacherWidget() {
    delete ui;
}
