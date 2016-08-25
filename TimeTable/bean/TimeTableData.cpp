#include "TimeTableData.h"

TimeTableData::TimeTableData() {
    col = 6;
    row = 10;
    rests.append(3);
    rests.append(6);

    dayTexts << "" << "星期一" << "星期二" << "星期三" << "星期四" << "星期五" << "星期六" << "星期天";
    timeTexts << ""
               << "08:00-08:40"
               << "08:50-09:30"
               << "09:40-10:30"
               << "08:00-08:30"
               << "08:00-08:30"
               << "08:00-08:30"
               << "08:00-08:30"
               << "08:00-08:30"
               << "08:00-08:30"
               << "08:00-08:30"
               << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "";
    restTexts << "课间休息" << "午间休息" << "休息" << "休息" << "" << "" << "" << "";
}

GETTER_IMPL(TimeTableData, row,       getRow, int)
GETTER_IMPL(TimeTableData, col,       getCol, int)
GETTER_IMPL(TimeTableData, rests,     getRests, QList<int>)
GETTER_IMPL(TimeTableData, dayTexts,  getDayTexts,  QList<QString>)
GETTER_IMPL(TimeTableData, timeTexts, getTimeTexts, QList<QString>)
GETTER_IMPL(TimeTableData, restTexts, getRestTexts, QList<QString>)

SETTER_IMPL(TimeTableData, rests,     setRests, QList<int>)
SETTER_IMPL(TimeTableData, dayTexts,  setDayTexts,  QList<QString>)
SETTER_IMPL(TimeTableData, timeTexts, setTimeTexts, QList<QString>)
SETTER_IMPL(TimeTableData, restTexts, getRestTexts, QList<QString>)

void TimeTableData::setRow(const int &row) {
    if (row <= 1) { return; }

    this->row = row;
    int len = rests.length();

    for (int i = len - 1; i >= 0; --i) {
        if (rests.at(i) >= row) {
            rests.removeAt(i);
        }
    }
}

void TimeTableData::setCol(const int &col) {
    if (col >= 2 && col <= 8) {
        this->col = col;
    }
}

bool TimeTableData::isRest(int row) const {
    foreach (int rest, rests) {
        if (rest == row) {
            return true;
        }
    }

    return false;
}

void TimeTableData::appendRest() {
    rests.append(row);
    setRow(row + 1);
}

void TimeTableData::removeRest() {
    if (!rests.isEmpty()) {
        rests.removeLast();
        setRow(row - 1);
    }
}

QString TimeTableData::getDayText(int col) const {
    return dayTexts.value(col, "");
}

QString TimeTableData::getTimeText(int row) const {
    return timeTexts.value(row, "");
}

QString TimeTableData::getRestText(int index) const {
    return restTexts.value(index, "");
}

void TimeTableData::setDayText(int index, const QString& title) {
    if (index >= 0 && index < dayTexts.size()) {
        dayTexts.replace(index, title);
    }
}

void TimeTableData::setTimeText(int index, const QString& title) {
    if (index >= 0 && index < timeTexts.size()) {
        timeTexts.replace(index, title);
    }
}

void TimeTableData::setRestText(int index, const QString& title) {
    if (index >= 0 && index < restTexts.size()) {
        restTexts.replace(index, title);
    }
}

QDataStream& operator<<(QDataStream &out, const TimeTableData &data) {
    out << data.row << data.col << data.rests << data.dayTexts << data.timeTexts << data.restTexts;
    return out;
}

QDataStream& operator>>(QDataStream &in, TimeTableData &data) {
    in >> data.row >> data.col >> data.rests >> data.dayTexts >> data.timeTexts >> data.restTexts;
    return in;
}
