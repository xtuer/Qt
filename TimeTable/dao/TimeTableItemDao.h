#ifndef TIMETABLEITEMDAO_H
#define TIMETABLEITEMDAO_H

class TimeTableItem;
class QString;
class QSqlQuery;
template<typename T> class QList;

class TimeTableItemDao {
public:
    static int insert(const TimeTableItem &item, int *primaryKey = 0);
    static int update(const TimeTableItem &item);
    static int removeById(int id);
    static int removeByClazzId(int id);
    static int removeByTeacherId(int id);
    static int findById(int id, TimeTableItem *item);
    static int findByClazzId(int id, QList<TimeTableItem*> *items);
    static int findByTeacherId(int id, QList<TimeTableItem*> *items);
    static int otherTimeTableItemCountAtTheSameTime(const TimeTableItem &item, int *count);
    static int updateClazzTimeTableItems(int clazzId, const QList<TimeTableItem*> &items);

private:
    static void mapRow(TimeTableItem *item, QSqlQuery *query);
};

#endif // TIMETABLEITEMDAO_H
