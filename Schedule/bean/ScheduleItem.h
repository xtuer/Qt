#ifndef SCHEDULEITEM_H
#define SCHEDULEITEM_H

#include <QString>

/**
 * @brief 课表项的类
 */
class ScheduleItem {
public:
    ScheduleItem();
    ScheduleItem(int day, int time, int classId);
    ScheduleItem(int id, int day, int time,
                 int classId, const QString &className,
                 int courseId, const QString &courseName,
                 int teacherId, const QString &teacherName,
                 bool editable, bool conflict);

    void initialize(int id, int day, int time,
                    int classId, const QString &className,
                    int courseId, const QString &courseName,
                    int teacherId, const QString &teacherName,
                    bool editable, bool conflict);

    QString toString() const;

    int id;
    int day;       // 星期
    int time;      // 第 time 节课
    int classId;   // 班级的 id
    int courseId;  // 课程的 id
    int teacherId; // 老师的 id

    QString className;   // 班级的名字
    QString courseName;  // 课程的名字
    QString teacherName; // 老师的名字

    bool editable; // 能否编辑
    bool conflict; // 排课是否冲突
};

#endif // SCHEDULEITEM_H
