#ifndef TEACHERDAO_H
#define TEACHERDAO_H

class Teacher;
class QSqlQuery;
template<typename T> class QList;

class TeacherDao {
public:
    static int findById(int id, Teacher *teacher);
    static int findAllTeachers(QList<Teacher*> *teachers);

private:
    static void mapRow(Teacher *teacher, QSqlQuery *query);
};

#endif // TEACHERDAO_H
