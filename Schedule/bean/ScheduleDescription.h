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

    qint32 days;              // 课表的行，表示时间
    qint32 times;             // 课表的列，表示星期
    QList<QString> dayTexts;  // 课程表的星期，第一行显示星期
    QList<QString> timeTexts; // 课程表的时间，第一列显示每节课的时间
    QList<qint32>  rests;     // 休息所在的行
    QString info; // 其他信息，例如课表的定制时间

    void setDayText(int dayIndex, const QString &text);   // 设置星期
    void setTimeText(int timeIndex, const QString &text); // 设置课程时间
    void setRestText(int row, const QString &text); // 修改休息的文本

    void increaseDay();  // 增加课表的行
    void decreaseDay();  // 减少课表的行
    void increaseTime(bool isRest = false); // 增加课表的行
    void decreaseTime(); // 减少课表的列
    void increaseRest(); // 增加一个 rest 到最后

    // 使用 QDataStream 读写 ScheduleDescription 的对象
    friend QDataStream& operator<<(QDataStream &stream, const ScheduleDescription &data);
    friend QDataStream& operator>>(QDataStream &stream, ScheduleDescription &data);
};

#endif // SCHEDULEDESCRIPTION_H
