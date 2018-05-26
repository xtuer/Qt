#include "Student.h"

Student::Student() {

}

QString Student::toString() const {
    QString s1 = QString("examUid: %1, examineeName: %2, idCardNo: %3, subjectCode: %4, subjectName: %5, ")
            .arg(examUid).arg(examineeName).arg(idCardNo).arg(subjectCode).arg(subjectName);
    QString s2 = QString("siteCode: %1, siteName: %2, roomCode: %3, seatCode: %4, periodUnitCode: %5, signedAt: %6")
            .arg(siteCode).arg(siteName).arg(roomCode).arg(seatCode).arg(periodUnitCode).arg(signedAt);

    return s1 + s2;
}
