#ifndef TEST_H
#define TEST_H

class Test {
public:
    static void test();
    static void testWriteScheduleDescription();
    static void testReadScheduleDescription();
    static void testSelectArrangeCourseByClassId();
    static void testInsertScheduleItem();
    static void testSelectScheduleItemsByClassId();
    static void testSortScheduleItems();
    static void testSelectAllClasses();
    static void testSelectAllTeachers();
    static void testSelectAllCourses();

    static void testRandem();
};

#endif // TEST_H
