#include "ScheduleDescription.h"

ScheduleDescription::ScheduleDescription() {
    dayCountOfWeek   = 5; // 每周默认上 5 天课，第一行是星期
    courseCountOfDay = 6; // 每天默认上 6 节课，第一列是课程的时间
    dayTexts << "" << "星期一" << "星期二" << "星期三" << "星期四" << "星期五" << "星期六" << "星期天";
    timeTexts << ""
               << "08:00-08:40"
               << "08:50-09:30"
               << "09:40-10:30"
               << "10:40-11:20"
               << "14:00-14:40"
               << "14:50-15:30"
               << "15:40-16:20"
               << "16:30-17:10"
               << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "";
}

void ScheduleDescription::setDayText(int dayIndex, const QString &text) {
    if (dayIndex > 0 && dayIndex < dayTexts.size()) {
        dayTexts.replace(dayIndex, text);
    }
}

void ScheduleDescription::setTimeText(int timeIndex, const QString &text) {
    if (timeIndex > 0 && timeIndex < timeTexts.size()) {
        timeTexts.replace(timeIndex, text);
    }
}

QDataStream& operator<<(QDataStream &stream, const ScheduleDescription &data) {
    stream << data.dayCountOfWeek << data.courseCountOfDay << data.dayTexts << data.timeTexts;
    return stream;
}

QDataStream& operator>>(QDataStream &stream, ScheduleDescription &data) {
    stream >> data.dayCountOfWeek >> data.courseCountOfDay >> data.dayTexts >> data.timeTexts;
    return stream;
}
