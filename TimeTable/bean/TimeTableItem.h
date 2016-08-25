#ifndef TIMETABLEITEM_H
#define TIMETABLEITEM_H

#include <QString>
#include <QByteArray>
#include <QDataStream>

class TimeTableItem {
public:
    TimeTableItem();

    int id;
    int clazzId;
    int courseId;
    int teacherId;

    QString clazzName;
    QString courseName;
    QString teacherName;

    int row;
    int col;
    bool editable;
};

#endif // TIMETABLEITEM_H
