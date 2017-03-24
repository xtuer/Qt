#ifndef STUDENT_H
#define STUDENT_H

#include <QString>

class Student {
public:
    Student();
    QString toString() const;

    QString examUid;        // 考生 ID
    QString examineeName;   // 考生名字
    QString idCardNo;       // 考生身份证号码
    QString subjectCode;    // 考试科目代号
    QString subjectName;    // 考试科目名称
    QString siteCode;       // 考试地点代号(学校)
    QString siteName;       // 考试地点名字
    QString roomCode;       // 考场代号
    QString seatCode;       // 座位号
    QString periodUnitCode; // 考期代号
    QString signedAt;       // 刷卡时间
};

#endif // STUDENT_H
