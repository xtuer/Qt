#include "TeacherDao.h"
#include "db/DBUtil.h"
#include "db/Sqls.h"
#include "bean/Teacher.h"

const char * const SQL_NAMESPACE_TEACHER = "Teacher";

QList<Teacher> TeacherDao::selectAllTeachers() {
    QString sql = Singleton<Sqls>::getInstance().getSql(SQL_NAMESPACE_TEACHER, "selectAllTeachers");
    return DBUtil::selectBeans(mapToTeacher, sql, QVariantMap());
}

QString TeacherDao::selectTeacherName(int teacherId) {
    QString sql = Singleton<Sqls>::getInstance().getSql(SQL_NAMESPACE_TEACHER, "selectTeacherName");
    QVariantMap params;
    params["teacherId"] = teacherId;

    return DBUtil::selectString(sql, params);
}

Teacher TeacherDao::mapToTeacher(const QVariantMap &rowMap) {
    Teacher teacher;

    teacher.id = rowMap.value("id").toInt();
    teacher.name = rowMap.value("name").toString();
    teacher.info = rowMap.value("info").toString();

    return teacher;
}
