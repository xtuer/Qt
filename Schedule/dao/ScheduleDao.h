#ifndef SCHEDULEDAO_H
#define SCHEDULEDAO_H

#include <QList>
#include <QString>
#include <QVariant>
#include <QVariantMap>

class ScheduleItem;

class ScheduleDao {
public:
    static QList<ScheduleItem> selectArrangedScheduleItemByClassId(int classId);
    static QList<ScheduleItem> selectScheduleItemsByClassId(int classId);
    static QList<ScheduleItem> selectScheduleItemsByTeacherId(int teacherId);
    static bool isScheduleItemConflict(const ScheduleItem &item);
    static void insertScheduleItem(const ScheduleItem &item);
    static void insertScheduleItems(const QList<ScheduleItem> &items);
    static void deleteScheduleItemsByClassId(int classId);

    static ScheduleItem mapToScheduleItem(const QVariantMap &rowMap);
};

#endif // SCHEDULEDAO_H
