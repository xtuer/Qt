#include "ClassCourseTeacherWidget.h"
#include "ui_ClassCourseTeacherWidget.h"
#include "util/UiUtil.h"

ClassCourseTeacherWidget::ClassCourseTeacherWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClassCourseTeacherWidget) {
    ui->setupUi(this);

    UiUtil::setWidgetPaddingAndSpacing(ui->widget, 0, 5);
    UiUtil::setWidgetPaddingAndSpacing(ui->widget_2, 0, 5);
    UiUtil::setWidgetPaddingAndSpacing(ui->widget_3, 0, 5);
}

ClassCourseTeacherWidget::~ClassCourseTeacherWidget() {
    delete ui;
}
