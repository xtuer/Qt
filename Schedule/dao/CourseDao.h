#ifndef COURSEDAO_H
#define COURSEDAO_H

#include <QList>
#include <QVariant>
#include <QVariantMap>

class Course;

class CourseDao {
public:
    static QList<Course> selectAllCourses();

    static Course mapToCourse(const QVariantMap &rowMap);
};

#endif // COURSEDAO_H
