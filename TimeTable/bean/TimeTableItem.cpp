#include "TimeTableItem.h"
#include <QDataStream>

TimeTableItem::TimeTableItem() {
    id = -1;
    clazzId = -1;
    courseId = -1;
    teacherId = -1;
    row = -1;
    col = -1;

    editable = true;
}
