#include "ExportUtil.h"
#include "bean/Clazz.h"
#include "bean/Teacher.h"
#include "bean/TimeTableData.h"
#include "bean/TimeTableItem.h"
#include "dao/TimeTableItemDao.h"
#include "dao/ClazzDao.h"
#include "dao/TeacherDao.h"
#include "util/SettingsUtil.h"
#include "Constants.h"

#include <QDebug>
#include <QList>
#include <QString>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QTextStream>
#include <QTextCodec>

QString ExportUtil::exportTimeTable(const TimeTableData &timeTableData,
                                    const QList<TimeTableItem*> &items,
                                    const QString &htmlTemplate,
                                    const QString &title,
                                    bool  forClazz) {
    QString table;

    int courseRow = 0;
    int rowCount = timeTableData.getRow();
    int colCount = timeTableData.getCol();
    int restIndex = 0;

    table += "<table>";
    for (int row = 0; row < rowCount; ++row) {
        // 添加休息条, 占一行
        if (timeTableData.isRest(row)) {
            table += QString("<tr><td class='%1' colspan='%2'>%3</td></tr>\n")
                     .arg(Constants::QSS_CLASS_REST)
                     .arg(colCount)
                     .arg(timeTableData.getRestText(restIndex));

            ++restIndex;

            continue;
        }

        if (row != 0) {
            ++courseRow; // 如果不是第一行且不是休息行,则这一行表示课程行
        }

        table += "<tr>";
        for (int col = 0; col < colCount; ++col) {
            // 第一行或第一列
            if (row == 0 || col == 0) {
                if (row == 0) {
                    // 第一行是星期的标题
                    table += QString("<td class='%1'>%2</td>\n")
                             .arg(Constants::QSS_CLASS_DAY_TEXT)
                             .arg(timeTableData.getDayText(col));
                } else {
                    // 第一列是时间的标题
                    table += QString("<td class='%1'>%2</td>\n")
                             .arg(Constants::QSS_CLASS_TIME_TEXT)
                             .arg(timeTableData.getTimeText(row));
                }
            }

            // 非第一行第一列表示课程
            if (row!=0 && col!=0) {
                QString courseName("&nbsp;"), clazzName("&nbsp;"), teacherName("&nbsp;");

                foreach (TimeTableItem *item, items) {
                    if (item->row == row && item->col == col) {
                        courseName  = item->courseName;

                        // 班级和教师的名字只显示一个
                        if (forClazz) {
                            teacherName = item->teacherName;
                        } else {
                            clazzName = item->clazzName;
                        }
                        break;
                    }
                }

                table += QString("<td class='%1'><p>%2</p><p>%3</p></td>\n")
                         .arg(Constants::QSS_CLASS_TIME_TABLE_ITEM)
                         .arg(courseName)
                         .arg(forClazz ? teacherName : clazzName);
            }
        }
        table += "</tr>\n";
    }

    table += "</table>\n";

    QString result(htmlTemplate);

    // Set title
    int titleIndex = result.indexOf("${title}");
    result.replace(titleIndex, 8, QString("<h2>%1</h2>").arg(title));

    int tableIndex = result.indexOf("${table}");
    result.replace(tableIndex, 8, table);

    return result;
}

void ExportUtil::saveTimeTable(const TimeTableData &timeTableData, const QList<int> ids, bool forClazz) {
    QString directoryPath = QFileDialog::getExistingDirectory(
                                NULL, QObject::tr("打开目录"),
                                QDir::currentPath(),
                                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (directoryPath.isEmpty()) { return; }
    QDir dir(directoryPath);

    QString htmlTemplate = SettingsUtil::getTimeTableHtmlTemplate();
    if (htmlTemplate.isEmpty()) { return; }

    foreach (int id, ids) {
        QList<TimeTableItem*> items;
        QString title;

        if (forClazz) {
            TimeTableItemDao::findByClazzId(id, &items);
        } else {
            TimeTableItemDao::findByTeacherId(id, &items);
        }

        if (items.size() > 0) {
            title = forClazz ? items.value(0)->clazzName : items.value(0)->teacherName;
        } else {
            if (forClazz) {
                Clazz clazz;
                ClazzDao::findById(id, &clazz);
                title = clazz.name;
            } else {
                Teacher teacher;
                TeacherDao::findById(id, &teacher);
                title = teacher.name;
            }

            qDebug() << __FILE__ << ":" << __LINE__ << ": No time table item found for: " << title;
        }

        QString filePath = dir.filePath(title + ".html");
        QString result = ExportUtil::exportTimeTable(timeTableData,
                                                     items,
                                                     htmlTemplate,
                                                     title,
                                                     forClazz);

        QFile file(filePath);

        if (!file.open(QIODevice::WriteOnly)) {
            qDebug() << "Cannot open the file: " << filePath;
            continue;
        }

        QTextStream out(&file);
        out.setCodec(QTextCodec::codecForName("UTF-8"));
        out << result;
        file.close();

        qDeleteAll(items);
    }
}
