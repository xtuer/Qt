#ifndef RESPONSEUTIL_H
#define RESPONSEUTIL_H

#include <QList>
#include <QString>

class Student;
class Site;
class PeriodUnit;

class ResponseUtil {
public:
    static QList<Student> responseToStudents(const QString &jsonResponse);
    static QList<Site> responseToSites(const QString &jsonResponse);
    static QList<PeriodUnit> responseToPeroidUnits(const QString &jsonResponse);
};

#endif // RESPONSEUTIL_H
