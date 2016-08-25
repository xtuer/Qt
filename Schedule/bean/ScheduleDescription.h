#ifndef SCHEDULEDESCRIPTION_H
#define SCHEDULEDESCRIPTION_H
#include <QList>
#include <QString>
#include <QDataStream>

/**
 * @brief 课程表结构描述的类
 */
class ScheduleDescription {
public:
    ScheduleDescription();

    qint32 dayCountOfWeek;    // 每星期上课的天数
    qint32 courseCountOfDay;  // 每天课程数
    QList<QString> timeTexts; // 课程表的时间，第一列显示每节课的时间
    QList<QString> dayTexts;  // 课程表的星期，第一行显示星期

    void setDayText(int dayIndex, const QString &text);   // 设置星期
    void setTimeText(int timeIndex, const QString &text); // 设置课程时间

    // 使用 QDataStream 读写 ScheduleDescription 的对象
    friend QDataStream& operator<<(QDataStream &stream, const ScheduleDescription &data);
    friend QDataStream& operator>>(QDataStream &stream, ScheduleDescription &data);
};

#endif // SCHEDULEDESCRIPTION_H
