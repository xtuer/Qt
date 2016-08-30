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
//#include <QPrinter>
//#include <QTextDocument>
//#include <QWebEnginePage>

void ScheduleExporter::exportClassSchedule(int classId, const QString &fileNamePrefix) {
    QString className = ClassDao::selectClassName(classId);

    if (className.isEmpty()) {
        qDebug() << QString("There is no class name found for class with ID %1").arg(classId);
        return;
    }

    QString fileName = fileNamePrefix + className + ".html";
    ScheduleDescription desc = ConfigUtil::readScheduleDescription();
    QList<ScheduleItem> items = ScheduleDao::selectScheduleItemsByClassId(classId);

    // 读取模版文件
    QString scheduleTemplate = readScheduleTemlate();
    // 导出为 HTML 课表
    QString scheduleHtml = exportScheduleToHtml(items, desc, "班级: " + className, false, true, scheduleTemplate);
    // 保存为 HTML 文件
    saveScheduleToHtml(scheduleHtml, fileName);
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
//    exportSchedule(items, desc, teacherName, true, false, fileName);

    // 读取模版文件
    QString scheduleTemplate = readScheduleTemlate();
    // 导出为 HTML 课表
    QString scheduleHtml = exportScheduleToHtml(items, desc, "老师: " + teacherName, true, false, scheduleTemplate);
    // 保存为 HTML 文件
    saveScheduleToHtml(scheduleHtml, fileName);
}

void ScheduleExporter::exportAllTeacherSchedule() {
    QList<Teacher> teachers = TeacherDao::selectAllTeachers();
    int count = 0;

    foreach (const Teacher &teacher, teachers) {
        exportTeacherSchedule(teacher.id, QString("老师-%1-").arg(++count));
    }
}

QString ScheduleExporter::exportScheduleToHtml(const QList<ScheduleItem> &items,
                                               const ScheduleDescription &desc,
                                               const QString &info,
                                               bool classVisible,
                                               bool teacherVisible,
                                               const QString &scheduleTemplate) {
    QString content = "";

    for (int time = 0; time <= desc.times; ++time) {
        content += "<tr>";
        for (int day = 0; day <= desc.days; ++day) {
            if (0 == time) {
                // 第一行为星期
                content += QString("<th class=\"day\">%1</th>").arg(desc.dayTexts.value(day));
            } else if (time > 0 && 0 == day) {
                if (desc.rests.contains(time)) {
                    // 休息
                    content += QString("<td class=\"rest\" colspan=\"%1\">%2</td>").arg(desc.days+1).arg(desc.timeTexts.value(time));
                } else {
                    // 时间
                    content += QString("<td class=\"time\">%1</td>").arg(desc.timeTexts.value(time));
                }
            } else if (time > 0 && day > 0 && !desc.rests.contains(time)) {
                // 课程
                ScheduleItem item = Util::findScheduleItemByDayAndTime(items, day, time);
                content += QString("<td><div class=\"schedule-item\">%1 %2 %3</div></td>")
                           .arg("<p><b>" + item.courseName + "</b></p>")
                           .arg(classVisible   ? ("<p>" + item.className + "</p>")   : "")
                           .arg(teacherVisible ? ("<p>" + item.teacherName + "</p>") : "");
            }
        }

        content += "</tr>";
    }

    double percent = 100.0 / (desc.days + 1);
    QString html = scheduleTemplate;
    html.replace("{{left-info}}", info);
    html.replace("{{right-info}}", desc.info);
    html.replace("{{th-width}}", QString("%1\%").arg(percent));
    html.replace("{{table-content}}", content);

    return html;
}

QString ScheduleExporter::readScheduleTemlate() {
    QFile scheduleTemplateFile("resources/schedule-template.html");
    if (!scheduleTemplateFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "打开文件 'resources/schedule-template'.html 出错";
        return "";
    }

    QString templateHtml = scheduleTemplateFile.readAll();
    scheduleTemplateFile.close();

    return templateHtml;
}

void ScheduleExporter::saveScheduleToHtml(const QString &scheduleHtml, const QString &fileName) {
    // 保存课表的文件
    QFile scheduleFile("课表/" + fileName);
    if (!scheduleFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << QString("打开文件 '课表/%1' 出错").arg(fileName);
        return;
    }
    QTextStream out(&scheduleFile);
    out.setCodec(QTextCodec::codecForName("UTF-8"));
    out << scheduleHtml;
    out.flush();
    scheduleFile.close();
}

void ScheduleExporter::saveScheduleToPdf(const QString &scheduleHtml, const QString &fileName) {
//    QWebEnginePage page;
//    page.setHtml("go.html", QUrl::fromLocalFile("/Users/Biao/Desktop"));
//    page.printToPdf(QString("/Users/Biao/Desktop/a.pdf"), QPageLayout( QPageSize( QPageSize::A4 ), QPageLayout::Portrait, QMarginsF()));
////    QPrinter printer;
////    printer.setOutputFormat(QPrinter::PdfFormat);
////    printer.setPaperSize(QPrinter::A4);
////    printer.setOrientation(QPrinter::Landscape);
////    printer.setOutputFileName("课表/" + fileName);

////    QTextDocument doc;
////    doc.setHtml(scheduleHtml);
////    doc.setPageSize(printer.pageRect().size()); // This is necessary if you want to hide the page number
////    doc.print(&printer);

////    qDebug() << doc.toHtml();
//    page.toHtml([](const QString &html){
//        qDebug() << html;
//    });
}
