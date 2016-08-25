#ifndef SQLUTIL_H
#define SQLUTIL_H

#include "util/Singleton.h"
#include <QHash>
#include <QString>
#include <QDomDocument>

class SqlUtil {
    SINGLETON(SqlUtil)
public:
    static QString sql(const QString &name);

private:
    void loadXml();
    void loadPrefixes();
    void loadSqls();

    QDomDocument doc;
    QHash<QString, QString> sqls;
    QHash<QString, QString> prefixes;
};

#endif // SQLUTIL_H
