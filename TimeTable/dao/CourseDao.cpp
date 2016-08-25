#include "CourseDao.h"

#include "bean/Course.h"
#include "dao/DaoTemplate.h"
#include "util/DBUtil.h"
#include "util/SqlUtil.h"

int CourseDao::findAllCourses(QList<Course*>* courses) {
    QString sql = SqlUtil::sql("findAllCourses");
    return DaoTemplate::selectBeans(courses, sql, mapRow);
}

void CourseDao::mapRow(Course* course, QSqlQuery* query) {
    course->id = query->value("id").toInt();
    course->name = query->value("name").toString();
}
