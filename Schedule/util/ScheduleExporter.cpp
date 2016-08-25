#include "ScheduleExporter.h"
#include "bean/Class.h"
#include "bean/Teacher.h"
#include "bean/ScheduleItem.h"
#include "bean/ScheduleDescription.h"
#include "dao/ScheduleDao.h"
#include "dao/ClassDao.h"
#include "dao/TeacherDao.h"
#include "util/Util.h"
#include "util/ConfigUtil.h"

#include <QDebug>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>

void ScheduleExporter::exportClassSchedule(int classId, const QString &fileNamePrefix) {
    QString className = ClassDao::selectClassName(classId);

    if (className.isEmpty()) {
        qDebug() << QString("There is no class name found for class with ID %1").arg(classId);
        return;
    }

    QString fileName = fileNamePrefix + className + ".html";
    ScheduleDescription desc = ConfigUtil::readScheduleDescription();
    QList<ScheduleItem> items = ScheduleDao::selectScheduleItemsByClassId(classId);
    exportSchedule(items, desc, className, false, true, fileName);
}

void ScheduleExporter::exportAllClassSchedule() {
    QList<Class> classes = ClassDao::selectAllClasses();
    int count = 0;

    foreach (const Class &clazz, classes) {
        exportClassSchedule(clazz.id, QString("班级-%1-").arg(++count));
    }
}

void ScheduleExporter::exportTeacherSchedule(int teacherId, const QString &fileNamePrefix) {
    QString teacherName = TeacherDao::selectTeacherName(teacherId);

    if (teacherName.isEmpty()) {
        qDebug() << QString("There is no teacher name found for teacher with ID %1").arg(teacherId);
        return;
    }

    QString fileName = fileNamePrefix + teacherName + ".html";
    ScheduleDescription desc = ConfigUtil::readScheduleDescription();
    QList<ScheduleItem> items = ScheduleDao::selectScheduleItemsByTeacherId(teacherId);
    exportSchedule(items, desc, teacherName, true, false, fileName);
}

void ScheduleExporter::exportAllTeacherSchedule() {
    QList<Teacher> teachers = TeacherDao::selectAllTeachers();
    int count = 0;

    foreach (const Teacher &teacher, teachers) {
        exportTeacherSchedule(teacher.id, QString("老师-%1-").arg(++count));
    }
}


void ScheduleExporter::exportSchedule(const QList<ScheduleItem> &items,
                                      const ScheduleDescription &desc,
                                      const QString &header,
                                      bool classVisible,
                                      bool teacherVisible,
                                      const QString &scheduleFileName) {
    QString h2 = QString("<h2>%1</h2>").arg(header);
    QString table("<table>");

    // 表头
    table += "<tr>";
    for (int i = 0; i <= desc.dayCountOfWeek; ++i) {
        table += QString("<th class=\"day\">%1</th>").arg(desc.dayTexts.value(i));
    }
    table += "</tr>";

    // 课程
    for (int time = 1; time <= desc.courseCountOfDay; ++time) {
        table += "<tr>";
        table += QString("<td class=\"time\">%1</td>").arg(desc.timeTexts.value(time)); // 时间
        for (int day = 1; day <= desc.dayCountOfWeek; ++day) {
            // 查找课程
            ScheduleItem item;
            foreach(const ScheduleItem &temp, items) {
                if (temp.day == day && temp.time == time) {
                    item = temp;
                }
            }

            table += QString("<td><div class=\"schedule-item\">%1 %2 %3</div></td>")
                    .arg("<p>" + item.courseName + "</p>")
                    .arg(classVisible   ? ("<p>" + item.className + "</p>")   : "")
                    .arg(teacherVisible ? ("<p>" + item.teacherName + "</p>") : ""); // 课程表项
        }
        table += "</tr>";
    }

    table += "</table>";

    QFile scheduleTemplateFile("resources/schedule-template.html");
    if (!scheduleTemplateFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "打开文件 'resources/schedule-template'.html 出错";
        return;
    }

    // 读取课表模版内容
    QString scheduleTemplate = scheduleTemplateFile.readAll();
    double percent = 100.0 / (desc.dayCountOfWeek + 1.0);
    scheduleTemplate.replace("{{th-width}}", QString("%1\%").arg(percent));
    scheduleTemplate.replace("{{template}}", h2 + table);
    scheduleTemplateFile.close();

    // 保存课表的文件
    QFile scheduleFile("课表/" + scheduleFileName);
    if (!scheduleFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << QString("打开文件 '课表/%1' 出错").arg(scheduleFileName);
        return;
    }
    QTextStream out(&scheduleFile);
    out.setCodec(QTextCodec::codecForName("UTF-8"));
    out << scheduleTemplate;
    out.flush();
    scheduleFile.close();
}
