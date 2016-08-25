#include "TimeTableItemDao.h"
#include "dao/DaoTemplate.h"
#include "bean/TimeTableItem.h"
#include "util/DBUtil.h"
#include "util/SqlUtil.h"

int TimeTableItemDao::insert(const TimeTableItem &item, int *primaryKey) {
    QString sql = SqlUtil::sql("insertTimeTableItem");

    QMap<QString, QVariant> params;
    params["clazzId"] = item.clazzId;
    params["courseId"] = item.courseId;
    params["teacherId"] = item.teacherId;
    params["row"] = item.row;
    params["col"] = item.col;
    params["editable"] = item.editable;

    return DaoTemplate::insert(sql, params, primaryKey);
}

int TimeTableItemDao::update(const TimeTableItem &item) {
    QString sql = SqlUtil::sql("updateTimeTableItem");

    QMap<QString, QVariant> params;
    params["clazzId"] = item.clazzId;
    params["courseId"] = item.courseId;
    params["teacherId"] = item.teacherId;
    params["row"] = item.row;
    params["col"] = item.col;
    params["editable"] = item.editable;
    params["id"] = item.id;

    return DaoTemplate::update(sql, params);
}

int TimeTableItemDao::removeById(int id) {
    QString sql = SqlUtil::sql("removeTimeTableItemById").arg(id);
    return DaoTemplate::update(sql);
}

int TimeTableItemDao::removeByClazzId(int id) {
    QString sql = SqlUtil::sql("removeTimeTableItemByClazzId").arg(id);
    return DaoTemplate::update(sql);
}

int TimeTableItemDao::removeByTeacherId(int id) {
    QString sql = SqlUtil::sql("removeTimeTableItemByTeacherId").arg(id);
    return DaoTemplate::update(sql);
}

int TimeTableItemDao::findById(int id, TimeTableItem *item) {
    QString sql = SqlUtil::sql("findTimeTableItemById").arg(id);
    return DaoTemplate::selectBean(item, sql, mapRow);
}

int TimeTableItemDao::findByClazzId(int clazzId, QList<TimeTableItem*> *items) {
    QString sql = SqlUtil::sql("findTimeTableItemByClazzId").arg(clazzId);
    return DaoTemplate::selectBeans(items, sql, mapRow);
}

int TimeTableItemDao::findByTeacherId(int teacherId, QList<TimeTableItem*> *items) {
    QString sql = SqlUtil::sql("findTimeTableItemByTeacherId").arg(teacherId);
    return DaoTemplate::selectBeans(items, sql, mapRow);
}

int TimeTableItemDao::otherTimeTableItemCountAtTheSameTime(const TimeTableItem &item, int *count) {
    QString sql = SqlUtil::sql("otherTimeTableItemCountAtTheSameTime");

    QMap<QString, QVariant> params;
    params["row"] = item.row;
    params["col"] = item.col;
    params["teacherId"] = item.teacherId;
    params["clazzId"] = item.clazzId;

    return DaoTemplate::selectInt(count, sql, params);
}

int TimeTableItemDao::updateClazzTimeTableItems(int clazzId, const QList<TimeTableItem*>& items) {
    DBUtil::getDatabase().transaction();
    removeByClazzId(clazzId);

    foreach (TimeTableItem *item, items) {
        insert(*item, NULL);
    }

    DBUtil::getDatabase().commit();

    return 0;
}

void TimeTableItemDao::mapRow(TimeTableItem *item, QSqlQuery *query) {
    item->id          = query->value("id").toInt();
    item->clazzId     = query->value("clazz_id").toInt();
    item->courseId    = query->value("course_id").toInt();
    item->teacherId   = query->value("teacher_id").toInt();
    item->clazzName   = query->value("clazz_name").toString();
    item->courseName  = query->value("course_name").toString();
    item->teacherName = query->value("teacher_name").toString();

    item->row      = query->value("row").toInt();
    item->col      = query->value("col").toInt();
    item->editable = query->value("editable").toBool();
}
