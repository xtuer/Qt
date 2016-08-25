#ifndef TEST_H
#define TEST_H

class Test {
public:
    void static test(); // 包含了所有希望的测试方法
    void static testTTiSwap(); // 测试交换2个 TimeTableItem
    void static testTimeTableData();
    void static testLoadSql();
    void static testTTiDao();
    void static testClazzDao();
    void static testCourseDao();
    void static testTeacherDao();
    void static testExportTimeTable();
};

#endif // TEST_H
