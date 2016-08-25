#ifndef SCHEDULEEXPORTER_H
#define SCHEDULEEXPORTER_H

#include <QList>
class ScheduleItem;
class ScheduleDescription;

class ScheduleExporter {
public:
    static void exportClassSchedule(int classId, const QString &fileNamePrefix = QString());
    static void exportAllClassSchedule();

    static void exportTeacherSchedule(int teacherId, const QString &fileNamePrefix = QString());
    static void exportAllTeacherSchedule();

    static void exportSchedule(const QList<ScheduleItem> &items,
                               const ScheduleDescription &desc,
                               const QString &header,
                               bool classVisible,
                               bool teacherVisible,
                               const QString &scheduleFileName);
};

#endif // SCHEDULEEXPORTER_H
