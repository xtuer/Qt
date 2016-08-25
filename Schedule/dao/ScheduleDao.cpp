#include "ScheduleDao.h"
#include "db/DBUtil.h"
#include "db/Sqls.h"
#include "bean/ScheduleItem.h"

const char * const SQL_NAMESPACE_SCHEDULE = "Schedule";

QList<ScheduleItem> ScheduleDao::selectArrangedScheduleItemByClassId(int classId) {
    QList<ScheduleItem> scheduleItems;
    QString sql = Singleton<Sqls>::getInstance().getSql(SQL_NAMESPACE_SCHEDULE, "selectArrangeCourseByClassId");
    QVariantMap params;
    params["classId"] = classId;
    QList<QVariantMap> result = DBUtil::selectMaps(sql, params);

    foreach (QVariantMap map, result) {
        ScheduleItem item;
        item.classId = map.value("classId").toInt();
        item.courseId = map.value("courseId").toInt();
        item.teacherId = map.value("teacherId").toInt();
        item.className = map.value("className").toString();
        item.courseName = map.value("courseName").toString();
        item.teacherName = map.value("teacherName").toString();

        int count = map.value("count").toInt();

        for (int i = 0; i < count; ++i) {
            scheduleItems << item;
        }
    }

    return scheduleItems;
}

QList<ScheduleItem> ScheduleDao::selectScheduleItemsByClassId(int classId) {
    QString sql = Singleton<Sqls>::getInstance().getSql(SQL_NAMESPACE_SCHEDULE, "selectScheduleItemsByClassId");
    QVariantMap params;
    params["classId"] = classId;

    return DBUtil::selectBeans(mapToScheduleItem, sql, params);
}

QList<ScheduleItem> ScheduleDao::selectScheduleItemsByTeacherId(int teacherId) {
    QString sql = Singleton<Sqls>::getInstance().getSql(SQL_NAMESPACE_SCHEDULE, "selectScheduleItemsByTeacherId");
    QVariantMap params;
    params["teacherId"] = teacherId;

    return DBUtil::selectBeans(mapToScheduleItem, sql, params);
}

// 同一个老师在同一天，同一个课时有多个排课时为冲突状态
bool ScheduleDao::isScheduleItemConflict(const ScheduleItem &item) {
    QString sql = Singleton<Sqls>::getInstance().getSql(SQL_NAMESPACE_SCHEDULE, "countOfScheduleItemInOtherClassByTeacherId");
    QVariantMap params;
    params["teacherId"] = item.teacherId;
    params["day"]  = item.day;
    params["time"] = item.time;
    params["classId"] = item.classId;

    int count = DBUtil::selectInt(sql, params);
    return count > 0;
}

void ScheduleDao::insertScheduleItem(const ScheduleItem &item) {
    QString sql = Singleton<Sqls>::getInstance().getSql(SQL_NAMESPACE_SCHEDULE, "insertScheduleItem");
    QVariantMap params;
    params["day"] = item.day;
    params["time"] = item.time;
    params["classId"] = item.classId;
    params["courseId"] = item.courseId;
    params["teacherId"] = item.teacherId;
    params["editable"] = item.editable;
//    params["conflict"] = item.conflict;

    DBUtil::insert(sql, params);
}

void ScheduleDao::insertScheduleItems(const QList<ScheduleItem> &items) {
    foreach (const ScheduleItem &item, items) {
        insertScheduleItem(item);
    }
}

void ScheduleDao::deleteScheduleItemsByClassId(int classId) {
    QString sql = Singleton<Sqls>::getInstance().getSql(SQL_NAMESPACE_SCHEDULE, "deleteScheduleItemsByClassId");
    QVariantMap params;
    params["classId"] = classId;

    DBUtil::update(sql, params);
}

ScheduleItem ScheduleDao::mapToScheduleItem(const QVariantMap &rowMap) {
    return ScheduleItem(rowMap.value("id").toInt(),
                        rowMap.value("day").toInt(),
                        rowMap.value("time").toInt(),
                        rowMap.value("classId").toInt(),
                        rowMap.value("className").toString(),
                        rowMap.value("courseId").toInt(),
                        rowMap.value("courseName").toString(),
                        rowMap.value("teacherId").toInt(),
                        rowMap.value("teacherName").toString(),
                        rowMap.value("editable").toBool(),
//                        rowMap.value("conflict").toBool());
                        false);
}
