#ifndef UTIL_H
#define UTIL_H

#include <QList>

class ScheduleItem;
class ScheduleDescription;

class Util {
public:
    static void exchangeScheduleItemDayAndTime(ScheduleItem *item1, ScheduleItem *item2);
    static ScheduleItem findScheduleItemByDayAndTime(const QList<ScheduleItem> &items, int day, int time);

    // 先按课时排，如果课时相同，再按星期排，也就是从左到右，从上到下
    static void sortScheduleItems(QList<ScheduleItem> *items);
};

#endif // UTIL_H
