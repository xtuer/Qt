#include "Util.h"
#include "bean/ScheduleItem.h"
#include "bean/ScheduleDescription.h"
#include <algorithm>

void Util::exchangeScheduleItemDayAndTime(ScheduleItem *item1, ScheduleItem *item2) {
    std::swap(item1->day, item2->day);
    std::swap(item1->time, item2->time);
}

QList<ScheduleItem> Util::fullScheduleItemsForClass(const QList<ScheduleItem> &items,
                                                    const ScheduleDescription &desc,
                                                    int classId) {
    QList<ScheduleItem> result;

    for (int day = 1; day <= desc.dayCountOfWeek; ++day) {
        for (int time = 1; time <= desc.courseCountOfDay; ++time) {
            ScheduleItem r(day, time, classId);

            // 查找课程，如果没有就用默认的
            foreach (const ScheduleItem& item, items) {
                if (item.day == day && item.time == time) {
                    r = item;
                    break;
                }
            }

            result << r;
        }
    }

    return result;
}

void Util::sortScheduleItems(QList<ScheduleItem> *items) {
    std::sort(items->begin(), items->end(), [](const ScheduleItem &i1, const ScheduleItem &i2) -> bool {
        return (i1.time == i2.time) ? (i1.day < i2.day) : (i1.time < i2.time);
    });
}
