#include "Util.h"
#include "bean/ScheduleItem.h"
#include "bean/ScheduleDescription.h"
#include <algorithm>

void Util::exchangeScheduleItemDayAndTime(ScheduleItem *item1, ScheduleItem *item2) {
    std::swap(item1->day, item2->day);
    std::swap(item1->time, item2->time);
}

ScheduleItem Util::findScheduleItemByDayAndTime(const QList<ScheduleItem> &items, int day, int time) {
    foreach (const ScheduleItem &item, items) {
        if (day == item.day && time == item.time) {
            return item;
        }
    }

    return ScheduleItem();
}

void Util::sortScheduleItems(QList<ScheduleItem> *items) {
    std::sort(items->begin(), items->end(), [](const ScheduleItem &i1, const ScheduleItem &i2) -> bool {
        return (i1.time == i2.time) ? (i1.day < i2.day) : (i1.time < i2.time);
    });
}
