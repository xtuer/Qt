#include "ResponseUtil.h"
#include "bean/Room.h"
#include "bean/Site.h"
#include "bean/Student.h"
#include "bean/PeriodUnit.h"
#include "util/JsonReader.h"

#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QDebug>

QList<Student> ResponseUtil::responseToStudents(const QString &jsonResponse) {
    QList<Student> students;
    JsonReader json(jsonResponse.toUtf8());
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

QList<Site> ResponseUtil::responseToSites(const QString &jsonResponse) {
    QList<Site> sites;
    JsonReader json(jsonResponse.toUtf8());
    QJsonArray siteJsonList = json.getJsonValue("siteList").toArray();

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

            s.rooms << r;
        }

        sites << s;
    }

    return sites;
}

QList<PeriodUnit> ResponseUtil::responseToPeroidUnits(const QString &jsonResponse) {
    QList<PeriodUnit> periodUnits;
    JsonReader json(jsonResponse.toUtf8());
    QJsonArray periodUnitJsonList = json.getJsonValue("periodUnitList").toArray();

    for (int i = 0; i < periodUnitJsonList.size(); ++i) {
        QJsonObject periodUnitJson = periodUnitJsonList.at(i).toObject();
        PeriodUnit pu;

        pu.periodUnitCode = periodUnitJson.value("periodUnitCode").toString().trimmed();
        pu.period         = periodUnitJson.value("period").toString().trimmed();
        pu.unit           = periodUnitJson.value("unit").toString().trimmed();
        pu.startTime      = periodUnitJson.value("startTime").toString().trimmed();
        pu.endTime        = periodUnitJson.value("endTime").toString().trimmed();

        periodUnits << pu;
    }

    return periodUnits;
}
