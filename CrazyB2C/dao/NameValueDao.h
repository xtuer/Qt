#ifndef NAMEVALUEDAO_H
#define NAMEVALUEDAO_H

#include <QList>
#include <QString>
#include <QVariant>
#include <QVariantMap>

class NameValue;

class NameValueDao {
public:
    static QString findValueByName(const QString name);
    static int  insert(const NameValue &nameValue);
    static bool update(const NameValue &nameValue);

private:
    static NameValue mapToNameValue(const QVariantMap &rowMap);
};

#endif // NAMEVALUEDAO_H
