#include "ScheduleDescription.h"
#include <QDebug>

ScheduleDescription::ScheduleDescription() {
    days  = 5; // 每周默认上 5 天课，第一行是星期, 6 列
    times = 6; // 每天默认上 6 节课，第一列是课程的时间，7 行
    dayTexts << "" << "星期一" << "星期二" << "星期三" << "星期四" << "星期五" << "星期六" << "星期天";
    timeTexts << "" << "" << "" << "" << "" << "" << "";
}

void ScheduleDescription::setDayText(int day, const QString &text) {
    if (day > 0 && day < dayTexts.size()) {
        dayTexts.replace(day, text);
    }
}

void ScheduleDescription::setTimeText(int time, const QString &text) {
    if (time > 0 && time < timeTexts.size()) {
        timeTexts.replace(time, text);
    }
}

void ScheduleDescription::setRestText(int row, const QString &text) {
    for (int i = 0; i < rests.size(); ++i) {
        if (rests[i] == row && row > 0 && row < timeTexts.size()) {
            timeTexts[row] = text;
            return;
        }
    }
}

void ScheduleDescription::increaseDay() {
    // 最多 7 天
    if (days < 7) {
        ++days;
    }
}

void ScheduleDescription::decreaseDay() {
    // 最少 5 天
    if (days > 5) {
        --days;
    }
}

void ScheduleDescription::increaseTime(bool isRest) {
    // 每天最多 12 节课
    if (times < 12) {
        ++times;
        timeTexts << "";

        if (isRest) {
            rests << times;
        }
    }
}

void ScheduleDescription::decreaseTime() {
    // 每天最少 4 节课
    if (times > 2) {
        --times;
        timeTexts.removeLast();
    }
}

void ScheduleDescription::increaseRest() {
    increaseTime(true);
}

QDataStream& operator<<(QDataStream &stream, const ScheduleDescription &data) {
    stream << data.days << data.times << data.dayTexts << data.timeTexts << data.rests << data.info;
    return stream;
}

QDataStream& operator>>(QDataStream &stream, ScheduleDescription &data) {
    stream >> data.days >> data.times >> data.dayTexts >> data.timeTexts >> data.rests >> data.info;
    return stream;
}
