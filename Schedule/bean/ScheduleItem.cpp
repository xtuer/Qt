#include "ScheduleItem.h"

ScheduleItem::ScheduleItem() {
    initialize(0, 0, 0, 0, "", 0, "", 0, "", true, false);
}

ScheduleItem::ScheduleItem(int day, int time, int classId) {
    initialize(0, day, time, classId, "", 0, "", 0, "", true, false);
}

ScheduleItem::ScheduleItem(int id, int day, int time,
                           int classId, const QString &className,
                           int courseId, const QString &courseName,
                           int teacherId, const QString &teacherName,
                           bool editable, bool conflict) {
    initialize(id, day, time, classId, className, courseId, courseName, teacherId, teacherName, editable, conflict);
}

void ScheduleItem::initialize(int id, int day, int time,
                              int classId, const QString &className,
                              int courseId, const QString &courseName,
                              int teacherId, const QString &teacherName,
                              bool editable, bool conflict) {
    this->id = id;
    this->day = day;
    this->time = time;
    this->classId = classId;
    this->className = className;
    this->courseId = courseId;
    this->courseName = courseName;
    this->teacherId = teacherId;
    this->teacherName = teacherName;
    this->editable = editable;
    this->conflict = conflict;
}

QString ScheduleItem::toString() const {
    return QString("id: %1, classId: %2, className: %3, courseId: %4, courseName: %5, teacherId: %6, teacherName: %7, day: %8, time: %9, editable: %10, conflict: %11")
            .arg(id).arg(classId).arg(className).arg(courseId).arg(courseName).arg(teacherId).arg(teacherName)
            .arg(day).arg(time).arg(editable).arg(conflict);
}
