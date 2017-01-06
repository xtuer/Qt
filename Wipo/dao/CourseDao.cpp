#include "CourseDao.h"
#include "bean/Course.h"
#include "bean/Courseware.h"
#include "util/Json.h"

#include <QJsonArray>

CourseDao::CourseDao(const QString &courseFilePath) {
    // 从文件读取课程到 Json 对象
    Json config(courseFilePath, true);

    // 加载模块对应的课程
    indexCourses           = loadModuleCourses(&config, "检索基础模块");
    analysisBaseCourses    = loadModuleCourses(&config, "分析基础模块");
    analysisAdvanceCourses = loadModuleCourses(&config, "分析提高模块");
    analysisInfoCourses    = loadModuleCourses(&config, "信息分析模块");
}

QList<Course> CourseDao::loadModuleCourses(Json *json, const QString &module) {
    QList<Course> courses; // 课程
    QJsonArray moduleCourses = json->getJsonArray(module); // 取得模块下的课程

    for (QJsonArray::const_iterator cIter = moduleCourses.begin(); cIter != moduleCourses.end(); ++cIter) {
        // [1] 加载课程
        Course course;
        QJsonObject c = (*cIter).toObject();
        course.name = json->getString("courseName", "", c); // 取得课程名字

        // [2] 加载课程下的课件
        QJsonArray cws = json->getJsonArray("coursewares", c);
        for (QJsonArray::const_iterator cwsIter = cws.begin(); cwsIter != cws.end(); ++cwsIter) {
            Courseware courseware;
            QJsonObject cw = (*cwsIter).toObject();
            courseware.name = json->getString("coursewareName", "", cw);
            courseware.video = json->getString("video", "", cw);

            course.coursewares << courseware;
        }

        courses << course;
    }

    return courses;
}
