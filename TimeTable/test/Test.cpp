#include "Test.h"
#include "bean/TimeTableItem.h"
#include "bean/TimeTableData.h"
#include "bean/Clazz.h"
#include "bean/Course.h"
#include "bean/Teacher.h"
#include "dao/TimeTableItemDao.h"
#include "dao/ClazzDao.h"
#include "dao/CourseDao.h"
#include "dao/TeacherDao.h"
#include "util/Util.h"
#include "util/SqlUtil.h"
#include "util/SettingsUtil.h"
#include "util/ExportUtil.h"

#include <QDebug>
#include <QString>
#include <QVariant>
#include <QList>
#include <QFile>

#define START_TEST(methodName) qDebug() << "\n\n" << QString("----------Start Test: %1()----------").arg(#methodName);
#define END_TEST(methodName)   qDebug() << QString("----------End   Test: %1()----------").arg(#methodName);

// 包含了所有希望的测试方法
void Test::test() {
//    testTTiSwap();
//    testTimeTableData();
//    testTTiDao();
//    testClazzDao();
//    testCourseDao();
//    testTeacherDao();
//    testLoadSql();
    testExportTimeTable();
}

// 测试交换2个 TimeTableItem
void Test::testTTiSwap() {
    START_TEST(testTTiSwap);

    TimeTableItem tti1;
    TimeTableItem tti2;

    tti1.clazzName = "One";
    tti1.row = 1;
    tti1.col = 1;

    tti2.clazzName = "Two";
    tti2.row = 2;
    tti2.col = 2;

    qDebug() << "Before swap";
    qDebug() << QString("Row: %1, Col: %2, ClassName: %3").arg(tti1.row).arg(tti1.col).arg(tti1.clazzName);
    qDebug() << QString("Row: %1, Col: %2, ClassName: %3").arg(tti2.row).arg(tti2.col).arg(tti2.clazzName);
    Util::swapTimeTableItem(&tti1, &tti2);

    qDebug() << "After swap";
    qDebug() << QString("Row: %1, Col: %2, ClassName: %3").arg(tti1.row).arg(tti1.col).arg(tti1.clazzName);
    qDebug() << QString("Row: %1, Col: %2, ClassName: %3").arg(tti2.row).arg(tti2.col).arg(tti2.clazzName);

    END_TEST(testTTiSwap);
}

void Test::testTimeTableData() {
    START_TEST(testTimeTableData)

    TimeTableData ttd, ttd2;
    ttd.setRow(2);
    ttd.setCol(1);
    qDebug() << QString("Row: %1, Col: %2").arg(ttd.getRow()).arg(ttd.getCol());

    QVariant var = QVariant::fromValue(ttd);
    qDebug() << var;

    ttd2 = var.value<TimeTableData>();
    qDebug() << QString("Row: %1, Col: %2").arg(ttd2.getRow()).arg(ttd2.getCol());

    END_TEST(testTimeTableData)
}

void Test::testTTiDao() {
    START_TEST(testTTiDao)

    // [[1]]
    TimeTableItem item;
    TimeTableItemDao::findById(1, &item);
    qDebug() << item.courseName;

    // [[2]]
    QList<TimeTableItem*> itemsByTeacher;
    TimeTableItemDao::findByTeacherId(1, &itemsByTeacher);
    foreach (TimeTableItem *i , itemsByTeacher) {
        qDebug() << i->courseName << ", " << i->teacherName;
    }
    qDeleteAll(itemsByTeacher);

    // [[3]]
    QList<TimeTableItem*> itemsByClazz;
    TimeTableItemDao::findByClazzId(1, &itemsByClazz);
    foreach (TimeTableItem *i , itemsByClazz) {
        qDebug() << i->courseName  << ", " << i->teacherName;
    }
    qDeleteAll(itemsByClazz);

    END_TEST(testTTiDao)
}

void Test::testClazzDao() {
    QList<Clazz*> clazzes;
    ClazzDao::findAllClazzes(&clazzes);

    foreach (Clazz *c, clazzes) {
        qDebug() << QString("ID: %1, Name: %2").arg(c->id).arg(c->name);
    }

    qDeleteAll(clazzes);
}

void Test::testCourseDao() {
    QList<Course*> courses;
    CourseDao::findAllCourses(&courses);

    foreach (Course *c, courses) {
        qDebug() << QString("ID: %1, Name: %2").arg(c->id).arg(c->name);
    }

    qDeleteAll(courses);
}

void Test::testTeacherDao() {
    QList<Teacher*> teachers;
    TeacherDao::findAllTeachers(&teachers);

    foreach (Teacher *t, teachers) {
        qDebug() << QString("ID: %1, Name: %2").arg(t->id).arg(t->name);
    }

    qDeleteAll(teachers);
}

void Test::testLoadSql() {
    START_TEST(testLoadSql)
    qDebug() << SqlUtil::sql("removeTimeTableItemByTeacherId");
    qDebug() << SqlUtil::sql("findTimeTableItemById");
    END_TEST(testLoadSql)
}

void Test::testExportTimeTable() {
    int clazzId = 1;
    QList<TimeTableItem*> items;
    TimeTableItemDao::findByClazzId(clazzId, &items);
    TimeTableData *timeTableData = new TimeTableData(SettingsUtil::getTimeTableData());

    QFile templateFile("printTemplate.html");
    if (!templateFile.open(QIODevice::ReadOnly)) {
        exit(4);
    }

    QString htmlTemplate(templateFile.readAll());
    QString html = ExportUtil::exportTimeTable(*timeTableData, items, htmlTemplate, "高一(1)班", false);

    qDebug() << html;
}
