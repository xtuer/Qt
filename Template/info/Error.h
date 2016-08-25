#ifndef ERROR_H
#define ERROR_H

class QString;

namespace Error {

extern const int     DB_CANNOT_OPEN;
extern const QString DB_CANNOT_OPEN_TEXT;

extern const int     SQL_FILE_NOT_FOUND;
extern const QString SQL_FILE_NOT_FOUND_TEXT;
extern const int     SQL_FILE_MALFORMED_FORMAT;
extern const QString SQL_FILE_MALFORMED_FORMAT_TEXT;
extern const int     SQL_INSERT_ERROR;
extern const QString SQL_INSERT_ERROR_TEXT;
extern const int     SQL_UPDATE_ERROR;
extern const QString SQL_UPDATE_ERROR_TEXT;
extern const int     SQL_SELECT_ERROR;
extern const QString SQL_SELECT_ERROR_TEXT;

extern const int     QSS_FILE_NOT_FOUND;
extern const QString QSS_FILE_NOT_FOUND_TEXT;

}

#endif // ERROR_H
