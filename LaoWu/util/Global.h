#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>

namespace Global {
    static const QString DATE_FORMAT("yyyy年MM月dd日");
    static const QString FILE_NAME_DATA("data.dat");
    static const QString FILE_NAME_NOTES("notes.dat");
    static const int     MAGIC_NUMBER_DATA = 5207;
    static const int     MAGIC_NUMBER_NOTES = 5208;

    int stringToInt(const QString &str, const QString &suffix);
    double stringToDouble(const QString &str, const QString &suffix);

    void backup();
}

#endif // GLOBAL_H
