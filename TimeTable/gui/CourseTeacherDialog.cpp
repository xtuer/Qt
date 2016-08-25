#include "CourseTeacherDialog.h"
#include "ui_CourseTeacherDialog.h"
#include "util/SettingsUtil.h"

CourseTeacherDialog::CourseTeacherDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CourseTeacherDialog) {
    ui->setupUi(this);

    SettingsUtil::restoreWindowGeometry(this->objectName(), this);
}

CourseTeacherDialog::~CourseTeacherDialog() {
    SettingsUtil::saveWindowGeometry(this->objectName(), this);
    delete ui;
}
