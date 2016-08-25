#ifndef COURSEDAO_H
#define COURSEDAO_H

class Course;
class QSqlQuery;
template<typename T> class QList;

class CourseDao {
public:
    static int findAllCourses(QList<Course*> *courses);

private:
    static void mapRow(Course *course, QSqlQuery *query);
};

#endif // COURSEDAO_H
