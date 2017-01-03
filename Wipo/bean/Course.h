#ifndef COURSE_H
#define COURSE_H

#include <QString>
#include <QList>

class Courseware;

class Course {
public:
    Course();

    QString name;
    QList<Courseware> coursewares;
};

#endif // COURSE_H
