#ifndef COURSEDAO_H
#define COURSEDAO_H

#include <QList>

class Json;
class Course;

class CourseDao {
public:
    CourseDao(const QString &courseFilePath);

    QList<Course> indexCourses;           // 检索基础模块的课程
    QList<Course> analysisBaseCourses;    // 分析基础模块的课程
    QList<Course> analysisAdvanceCourses; // 分析提高模块的课程
    QList<Course> analysisInfoCourses;    // 信息分析模块的课程

private:
    QList<Course> loadModuleCourses(Json *json, const QString &module);
};

#endif // COURSEDAO_H
