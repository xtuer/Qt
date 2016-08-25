#ifndef TEACHERDAO_H
#define TEACHERDAO_H

#include <QList>
#include <QVariant>
#include <QVariantMap>

class Teacher;

class TeacherDao {
public:
    static QList<Teacher> selectAllTeachers();
    static QString selectTeacherName(int teacherId);

    static Teacher mapToTeacher(const QVariantMap &rowMap);
};

#endif // TEACHERDAO_H
