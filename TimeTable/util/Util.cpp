#include "Util.h"
#include "bean/TimeTableItem.h"

void Util::swapTimeTableItem(TimeTableItem *item1, TimeTableItem *item2) {
    TimeTableItem temp = *item1;
    *item1 = *item2;
    *item2 = temp;

    // 交换位置, item 保持原来的位置
    int ti = item1->row;
    item1->row = item2->row;
    item2->row = ti;

    ti = item1->col;
    item1->col = item2->col;
    item2->col = ti;
}

