#include "Test.h"
#include "bean/ScheduleDescription.h"
#include "bean/ScheduleItem.h"
#include "bean/Class.h"
#include "bean/Course.h"
#include "bean/Teacher.h"
#include "dao/ClassDao.h"
#include "dao/CourseDao.h"
#include "dao/TeacherDao.h"
#include "dao/ScheduleDao.h"
#include "util/Util.h"

#include <QFile>
#include <QList>
#include <QDebug>
#include <QDataStream>
#include <QtGlobal>
#include <QDateTime>
#include <QList>

#define START_TEST(methodName) qDebug() << QString("----------Start Test: %1()----------").arg(#methodName);
#define END_TEST(methodName)   qDebug() << QString("----------End   Test: %1()----------").arg(#methodName) << "\n\n";

void Test::test() {
    testSelectAllCourses();
}

void Test::testWriteScheduleDescription() {
    START_TEST(testWriteScheduleDescription);

    ScheduleDescription desc;
    desc.days = 100;
    desc.dayTexts << "无效时间";

    QFile file("test/schedule_description.data");
    file.open(QIODevice::WriteOnly);
    QDataStream stream(&file);
    stream << desc;
    file.close();

    END_TEST(testWriteScheduleDescription);
}

void Test::testReadScheduleDescription() {
    START_TEST(testReadScheduleDescription);

    ScheduleDescription desc;
    QFile file("test/schedule_description.data");
    file.open(QIODevice::ReadOnly);
    QDataStream stream(&file);
    stream >> desc;
    file.close();

    qDebug() << desc.days << desc.dayTexts;

    END_TEST(testReadScheduleDescription);
}

void Test::testSelectArrangeCourseByClassId() {
    START_TEST(testSelectArrangeCourseByClassId);

    QList<ScheduleItem> scheduleItems = ScheduleDao::selectArrangedScheduleItemByClassId(5);

    foreach (ScheduleItem item, scheduleItems) {
        qDebug() << item.className << ", " << item.courseName << ", " << item.teacherName;
    }

    END_TEST(testSelectArrangeCourseByClassId);
}

void Test::testInsertScheduleItem() {
    START_TEST(testInsertScheduleItem);

    ScheduleItem item;
    item.day = 5;
    item.time = 3;
    item.classId = 1;
    item.courseId = 3;
    item.teacherId = 1;
    item.editable = true;
    item.conflict = false;

    ScheduleDao::insertScheduleItem(item);

    END_TEST(testInsertScheduleItem);
}

void Test::testSelectScheduleItemsByClassId() {
    START_TEST(testSelectScheduleItemsByClassId);

    QList<ScheduleItem> items = ScheduleDao::selectScheduleItemsByClassId(1);

    foreach (const ScheduleItem &item, items) {
        qDebug() << item.toString();
    }

    END_TEST(testSelectScheduleItemsByClassId);
}

void Test::testSortScheduleItems() {
    START_TEST(testSortScheduleItems);

    QList<ScheduleItem> items;
    items << ScheduleItem(1, 1, 0) << ScheduleItem(2, 2, 0) << ScheduleItem(2, 1, 0) << ScheduleItem(1, 2, 0);
    Util::sortScheduleItems(&items);

    foreach (const ScheduleItem &item, items) {
        qDebug() << item.toString();
    }

    END_TEST(testSortScheduleItems);
}

void Test::testSelectAllClasses() {
    START_TEST(testSelectAllClasses);

    QList<Class> clazzes = ClassDao::selectAllClasses();

    foreach (const Class &clazz, clazzes) {
        qDebug() << QString("ID: %1, Name: %2").arg(clazz.id).arg(clazz.name);
    }

    END_TEST(testSelectAllClasses);
}

void Test::testSelectAllTeachers() {
    START_TEST(testSelectAllTeachers);

    QList<Teacher> teachers = TeacherDao::selectAllTeachers();

    foreach (const Teacher &teacher, teachers) {
        qDebug() << QString("ID: %1, Name: %2, Info: %3").arg(teacher.id).arg(teacher.name).arg(teacher.info);
    }

    END_TEST(testSelectAllTeachers);
}

void Test::testSelectAllCourses() {
    START_TEST(testSelectAllCourses);

    QList<Course> courses = CourseDao::selectAllCourses();

    foreach (const Course &course, courses) {
        qDebug() << QString("ID: %1, Name: %2").arg(course.id).arg(course.name);
    }

    END_TEST(testSelectAllCourses);
}

void Test::testRandem() {
    START_TEST(testRandem);

    qsrand(QDateTime::currentMSecsSinceEpoch());

    for (int i = 0; i < 10; ++i) {
        qDebug() << qrand() % 5;
    }

    END_TEST(testRandem);
}
