#include "Error.h"
#include <QString>

namespace Error {

const int     DB_CANNOT_OPEN = 101;
const QString DB_CANNOT_OPEN_TEXT("打不开数据库");

const int     SQL_FILE_NOT_FOUND = 102;
const QString SQL_FILE_NOT_FOUND_TEXT("找不到 SQL 文件");
const int     SQL_FILE_MALFORMED_FORMAT = 103;
const QString SQL_FILE_MALFORMED_FORMAT_TEXT("SQL 文件格式不对");
const int     SQL_INSERT_ERROR = 104;
const QString SQL_INSERT_ERROR_TEXT("");
const int     SQL_UPDATE_ERROR = 105;
const QString SQL_UPDATE_ERROR_TEXT("");
const int     SQL_SELECT_ERROR = 106;
const QString SQL_SELECT_ERROR_TEXT("");

const int     QSS_FILE_NOT_FOUND = 110;
const QString QSS_FILE_NOT_FOUND_TEXT("找不到 QSS 文件");
}
