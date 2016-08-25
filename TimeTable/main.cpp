/*
                   _ooOoo_
                  o8888888o
                  88" . "88
                  (| -_- |)
                  O\  =  /O
               ____/`---'\____
             .'  \\|     |//  `.
            /  \\|||  :  |||//  \
           /  _||||| -:- |||||-  \
           |   | \\\  -  /// |   |
           | \_|  ''\---/''  |   |
           \  .-\__  `-`  ___/-. /
         ___`. .'  /--.--\  `. . __
      ."" '<  `.___\_<|>_/___.'  >'"".
     | | :  `- \`.;`\ _ /`;.`/ - ` : | |
     \  \ `-.   \_ __\ /__ _/   .-` /  /
======`-.____`-.___\_____/___.-`____.-'======
                   `=---='
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
         佛祖保佑       永无BUG
*/

#include "gui/MainWidget.h"
#include "gui/DesignTimeTableWidget.h"
#include "gui/ManageClazzWidget.h"
#include "gui/ManageCourseWidget.h"
#include "gui/ManageTeacherWidget.h"
#include "gui/ManageClazzCourseTeacherWidget.h"
#include "gui/CourseTeacherWidget.h"
#include "gui/CentralWidget.h"
#include "gui/TeacherTimeTableWidget.h"

#include "util/UiUtil.h"
#include "test/Test.h"

#include <QApplication>

int main(int argc, char *argv[]) {
//    Test::test();

    QApplication a(argc, argv);
    UiUtil::loadStyleSheet();

    CentralWidget w;
    w.show();

    return a.exec();
}
