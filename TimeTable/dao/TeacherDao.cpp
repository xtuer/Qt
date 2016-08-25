#include "TeacherDao.h"

#include "bean/Teacher.h"
#include "dao/DaoTemplate.h"
#include "util/DBUtil.h"
#include "util/SqlUtil.h"

int TeacherDao::findById(int id, Teacher* teacher) {
    QString sql = SqlUtil::sql("findTeacherById").arg(id);
    return DaoTemplate::selectBean(teacher, sql, mapRow);
}

int TeacherDao::findAllTeachers(QList<Teacher*>* teachers) {
    QString sql = SqlUtil::sql("findAllTeachers");
    return DaoTemplate::selectBeans(teachers, sql, mapRow);
}

void TeacherDao::mapRow(Teacher* teacher, QSqlQuery* query) {
    teacher->id = query->value("id").toInt();
    teacher->name = query->value("name").toString();
}
