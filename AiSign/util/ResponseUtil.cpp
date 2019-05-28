#include "ResponseUtil.h"
#include "bean/Room.h"
#include "bean/Site.h"
#include "bean/Student.h"
#include "bean/Unit.h"
#include "util/Json.h"

#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QDebug>

// HTTP 的响应转换为学生 Student 数组
QList<Student> ResponseUtil::responseToStudents(const QString &jsonResponse) {
    QList<Student> students;
    Json json(jsonResponse.toUtf8());
    QJsonArray studentJsonList = json.getJsonValue("roomEnrollmentList").toArray();

    for (int i = 0; i < studentJsonList.size(); ++i) {
        QJsonObject studentJson = studentJsonList.at(i).toObject();
        Student s;

        s.examUid        = studentJson.value("examUId").toString().trimmed();
        s.examineeName   = studentJson.value("examineeName").toString().trimmed();
        s.idCardNo       = studentJson.value("idCardNo").toString().trimmed();
        s.subjectCode    = studentJson.value("subjectCode").toString().trimmed();
        s.subjectName    = studentJson.value("subjectName").toString().trimmed();
        s.siteCode       = studentJson.value("siteCode").toString().trimmed();
        s.siteName       = studentJson.value("siteName").toString().trimmed();
        s.roomCode       = studentJson.value("roomCode").toString().trimmed();
        s.seatCode       = studentJson.value("seatCode").toString().trimmed();
        s.periodUnitCode = studentJson.value("periodUnitCode").toString().trimmed();
        s.signedAt       = studentJson.value("signedAt").toString().trimmed();

        students << s;
    }

    return students;
}

// HTTP 的响应转换为考点 Site 数组
QList<Site> ResponseUtil::responseToSites(const QString &jsonResponse) {
    QList<Site> sites;
    Json json(jsonResponse.toUtf8());
    QJsonArray siteJsonList = json.getJsonValue("signInSiteAndRoomList").toArray();

    for (int i = 0; i < siteJsonList.size(); ++i) {
        QJsonObject siteJson = siteJsonList.at(i).toObject();
        Site s;

        s.siteCode    = siteJson.value("siteCode").toString().trimmed();
        s.siteName    = siteJson.value("siteName").toString().trimmed();
        s.siteAddress = siteJson.value("siteAddress").toString().trimmed();

        QJsonArray roomJsonList = siteJson.value("roomList").toArray();
        for (int j = 0; j < roomJsonList.size(); ++j) {
            QJsonObject roomJson = roomJsonList.at(j).toObject();
            Room r;

            r.roomCode     = roomJson.value("roomCode").toString().trimmed();
            r.roomLocation = roomJson.value("roomLocation").toString().trimmed();
            r.seatNum      = roomJson.value("seatNum").toInt(0);
            r.needSignIn   = roomJson.value("needSignIn").toBool(false);

            s.rooms << r;
        }

        sites << s;
    }

    return sites;
}

// HTTP 的响应转换为考试单元 Unit 数组
QList<Unit> ResponseUtil::responseToUnits(const QString &jsonResponse) {
    QList<Unit> units;
    Json json(jsonResponse.toUtf8());
    QJsonArray periodUnitJsonList = json.getJsonValue("signInExamUnitList").toArray();

    for (int i = 0; i < periodUnitJsonList.size(); ++i) {
        QJsonObject periodUnitJson = periodUnitJsonList.at(i).toObject();
        Unit u;

        u.examCode  = periodUnitJson.value("examCode").toString().trimmed();
        u.unit      = periodUnitJson.value("unit").toString().trimmed();
        u.startTime = periodUnitJson.value("startTime").toString().trimmed();
        u.endTime   = periodUnitJson.value("endTime").toString().trimmed();

        units << u;
    }

    return units;
}
