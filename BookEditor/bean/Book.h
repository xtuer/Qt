#ifndef BOOKINFO_H
#define BOOKINFO_H

#include <QString>

extern const int ROLE_TYPE;        // 表示类型的 role
extern const int ROLE_CODE;        // 表示编码的 role
extern const int ROLE_COVER;       // 表示封面的 role
extern const QString TYPE_PHASE;   // 教学阶段
extern const QString TYPE_SUBJECT; // 学科
extern const QString TYPE_VERSION; // 版本
extern const QString TYPE_BOOK;    // 教材

// 存储 code 和 info 对
class CodeInfo {
public:
    QString code;
    QString info; // 阶段 > 学科 > 版本

    CodeInfo(const QString &code, const QString &info);
};
#endif // BOOKINFO_H
