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

    static QString exportScheduleToHtml(const QList<ScheduleItem> &items,
                                        const ScheduleDescription &desc,
                                        const QString &info,
                                        bool classVisible,
                                        bool teacherVisible,
                                        const QString &scheduleTemplate);

    static QString readScheduleTemlate();
    static void saveScheduleToHtml(const QString &scheduleHtml, const QString &fileName);
    static void saveScheduleToPdf(const QString &scheduleHtml, const QString &fileName);
};

#endif // SCHEDULEEXPORTER_H
