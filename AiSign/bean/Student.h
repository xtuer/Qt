#ifndef STUDENT_H
#define STUDENT_H

#include <QString>

/**
 * 学生
 */
class Student {
public:
    Student();
    QString toString() const;

    QString examUid;      // 考生 ID，又叫考籍号
    QString examineeName; // 考生名字
    QString idCardNo;     // 考生身份证号码
    QString subjectCode;  // 考试科目代号
    QString subjectName;  // 考试科目名称
    QString siteCode;     // 考试地点代号(学校)
    QString siteName;     // 考试地点名字
    QString roomCode;     // 考场代号
    QString seatCode;     // 座位号
    QString unit;         // 考期代号
    QString signedAt;     // 刷卡时间
};

#endif // STUDENT_H
