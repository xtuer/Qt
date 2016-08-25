#include "CourseDao.h"
#include "db/DBUtil.h"
#include "db/Sqls.h"
#include "bean/Course.h"

const char * const SQL_NAMESPACE_COURSE = "Course";

QList<Course> CourseDao::selectAllCourses() {
    QString sql = Singleton<Sqls>::getInstance().getSql(SQL_NAMESPACE_COURSE, "selectAllCourses");
    return DBUtil::selectBeans(mapToCourse, sql, QVariantMap());
}

Course CourseDao::mapToCourse(const QVariantMap &rowMap) {
    Course course;

    course.id = rowMap.value("id").toInt();
    course.name = rowMap.value("name").toString();

    return course;
}
