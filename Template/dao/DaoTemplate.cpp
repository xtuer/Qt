#include "DaoTemplate.h"
#include "util/DBUtil.h"
#include "util/SettingUtil.h"

int DaoTemplate::insert(const QString &sql, const QMap<QString, QVariant> &params) {
    QSqlQuery query(DBUtil::getDatabase());
    query.prepare(sql);
    bindValues(&query, params);

    if (query.exec()) {
        debugSql(query, params);
        return query.lastInsertId().toInt(); // 返回插入行的主键
    } else {
        debugSql(query, params);
        return -1;
    }
}

bool DaoTemplate::update(const QString &sql, const QMap<QString, QVariant> &params) {
    QSqlQuery query(DBUtil::getDatabase());
    query.prepare(sql);
    bindValues(&query, params);

    if (query.exec()) {
        debugSql(query, params);
        return true;
    } else {
        debugSql(query, params);
        return false;
    }
}

int DaoTemplate::selectInt(const QString &sql, const QMap<QString, QVariant> &params) {
    return selectVariant(sql, params).toInt();
}

qint64 DaoTemplate::selectInt64(const QString &sql, const QMap<QString, QVariant> &params) {
    return selectVariant(sql, params).toLongLong();
}

QString DaoTemplate::selectString(const QString &sql, const QMap<QString, QVariant> &params) {
    return selectVariant(sql, params).toString();
}

QDate DaoTemplate::selectDate(const QString &sql, const QMap<QString, QVariant> &params) {
    return selectVariant(sql, params).toDate();
}

QDateTime DaoTemplate::selectDateTime(const QString &sql, const QMap<QString, QVariant> &params) {
    return selectVariant(sql, params).toDateTime();
}

QVariant DaoTemplate::selectVariant(const QString &sql, const QMap<QString, QVariant> &params) {
    QSqlQuery query(DBUtil::getDatabase());
    query.prepare(sql);
    bindValues(&query, params);

    if (query.exec() && query.next()) {
        debugSql(query, params);
        return query.value(0);
    } else {
        debugSql(query, params);
        return QVariant();
    }
}

QStringList DaoTemplate::selectStrings(const QString &sql, const QMap<QString, QVariant> &params) {
    QSqlQuery query(DBUtil::getDatabase());
    query.prepare(sql);
    bindValues(&query, params);

    if (query.exec()) {
        debugSql(query, params);
        QStringList strings;

        while (query.next()) {
            strings.append(query.value(0).toString());
        }

        return strings;
    } else {
        debugSql(query, params);
        return QStringList();
    }
}

QMap<QString, QVariant> DaoTemplate::selectMap(const QString &sql, const QMap<QString, QVariant> &params) {
    QSqlQuery query(DBUtil::getDatabase());
    query.prepare(sql);
    bindValues(&query, params);

    if (query.exec()) {
        debugSql(query, params);
        return queryToMap(&query);
    } else {
        debugSql(query, params);
        return QMap<QString, QVariant>();
    }
}

QList<QMap<QString, QVariant> > DaoTemplate::selectMaps(const QString &sql, const QMap<QString, QVariant> &params) {
    QSqlQuery query(DBUtil::getDatabase());
    query.prepare(sql);
    bindValues(&query, params);

    if (query.exec()) {
        debugSql(query, params);
        return queryToMaps(&query);
    } else {
        debugSql(query, params);
        return QList<QMap<QString, QVariant> >();
    }
}

QStringList DaoTemplate::getFieldNames(const QSqlQuery &query) {
    QSqlRecord record = query.record();
    QStringList names;
    int count = record.count();

    for (int i = 0; i < count; ++i) {
        names << record.fieldName(i);
    }

    return names;
}

void DaoTemplate::bindValues(QSqlQuery *query, const QMap<QString, QVariant> &params) {
    for (QMap<QString, QVariant>::const_iterator i=params.constBegin(); i!=params.constEnd(); ++i) {
        query->bindValue(":" + i.key(), i.value());
    }
}

QMap<QString, QVariant> DaoTemplate::queryToMap(QSqlQuery *query, const QStringList &fieldNames, bool usingNext) {
    QMap<QString, QVariant> rowMap;
    QStringList names = (fieldNames.size() != 0) ? fieldNames : getFieldNames(*query);

    // 如果 usingNext 为 true，则调用 query->next()
    // 如果 usingNext 为 false，则不调用 query->next()
    if (!usingNext || query->next()) {
        foreach (QString fieldName, names) {
            rowMap.insert(fieldName, query->value(fieldName));
        }
    }

    return rowMap;
}

QList<QMap<QString, QVariant> > DaoTemplate::queryToMaps(QSqlQuery *query) {
    QList<QMap<QString, QVariant> > rowMaps;
    QStringList fieldNames = getFieldNames(*query);

    while (query->next()) {
        rowMaps.append(queryToMap(query, fieldNames, false));
    }

    return rowMaps;
}

void DaoTemplate::debugSql(const QSqlQuery &query, const QMap<QString, QVariant> &params) {
    if (SettingUtil::getInstance().isSqlDebug()) {
        qDebug() << "SQL Error :" << query.lastError().text().trimmed();
        qDebug() << "SQL Detail:" << query.lastQuery();
        qDebug() << "SQL Params:" << params;
    }
}

