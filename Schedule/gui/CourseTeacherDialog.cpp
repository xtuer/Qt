#include "CourseTeacherDialog.h"
#include "ui_CourseTeacherDialog.h"
#include "util/UiUtil.h"
#include "bean/Course.h"
#include "bean/Teacher.h"
#include "dao/CourseDao.h"
#include "dao/TeacherDao.h"
#include "Constants.h"
#include "gui/DragableCourseTeacherLabel.h"

#include <QList>

CourseTeacherDialog::CourseTeacherDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::CourseTeacherDialog) {
    ui->setupUi(this);
//    setAttribute(Qt::WA_DeleteOnClose, true);
}

CourseTeacherDialog::~CourseTeacherDialog() {
    delete ui;
}

void CourseTeacherDialog::showEvent(QShowEvent *event) {
    Q_UNUSED(event)
    loadCourses();
    loadTeachers();
}

void CourseTeacherDialog::loadCourses() {
    QList<Course> courses = CourseDao::selectAllCourses();

    UiUtil::deleteChildrenAndLayout(ui->courseWidget);
    QGridLayout *layout = new QGridLayout();

    for (int i = 0; i < courses.size(); ++i) {
        const Course &course = courses.at(i);
        QWidget *courseLabel = new DragableCourseTeacherLabel(course.id, course.name, Constants::MIME_TYPE_COURSE);
        layout->addWidget(courseLabel, i/4, i%4); // 一行 4 个
    }

    layout->setContentsMargins(0, 0, 0, 0);
    ui->courseWidget->setLayout(layout);
}

void CourseTeacherDialog::loadTeachers() {
    QList<Teacher> teachers = TeacherDao::selectAllTeachers();

    UiUtil::deleteChildrenAndLayout(ui->teacherWidget);
    QGridLayout *layout = new QGridLayout();

    for (int i = 0; i < teachers.size(); ++i) {
        const Teacher &teacher = teachers.at(i);
        QWidget *teacherLabel = new DragableCourseTeacherLabel(teacher.id, teacher.name, Constants::MIME_TYPE_TEACHER);
        layout->addWidget(teacherLabel, i/4, i%4); // 一行 4 个
    }

    layout->setContentsMargins(0, 0, 0, 0);
    ui->teacherWidget->setLayout(layout);
}
