#include "Book.h"

const int ROLE_TYPE  = Qt::UserRole + 2; // 表示类型的 role
const int ROLE_CODE  = Qt::UserRole + 3; // 表示编码的 role
const int ROLE_COVER = Qt::UserRole + 4; // 表示封面的 role
const QString TYPE_PHASE   = "phase";   // 教学阶段
const QString TYPE_SUBJECT = "subject"; // 学科
const QString TYPE_VERSION = "version"; // 版本
const QString TYPE_BOOK    = "book";    // 教材

CodeInfo::CodeInfo(const QString &code, const QString &info) : code(code), info(info) {

}
