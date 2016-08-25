#include "DaoTemplate.h"

int DaoTemplate::insert(const QString &sql, const QMap<QString, QVariant> &params, int *primaryKey) {
    QSqlQuery query(DBUtil::getDatabase());
    query.prepare(sql);
    bindValues(&query, params);

    if (query.exec()) {
        if (NULL != primaryKey) {
            *primaryKey = query.lastInsertId().toInt();
        }
        return 0;
    } else {
        qDebug() << query.lastError().text();
        return 1;
    }
}

int DaoTemplate::update(const QString &sql, const QMap<QString, QVariant> &params) {
    QSqlQuery query(DBUtil::getDatabase());
    query.prepare(sql);
    bindValues(&query, params);

    if (query.exec()) {
        return 0;
    } else {
        qDebug() << query.lastError().text();
        return 1;
    }
}

int DaoTemplate::selectInt(int *result, const QString &sql, const QMap<QString, QVariant> &params) {
    return selectOneOfType(result, QVariant::Int, sql, params);
}

int DaoTemplate::selectInt64(qint64 *result, const QString &sql, const QMap<QString, QVariant> &params) {
    return selectOneOfType(result, QVariant::LongLong, sql, params);
}

int DaoTemplate::selectVariant(QVariant *var, const QString &sql, const QMap<QString, QVariant> &params) {
    return selectOneOfType(var, QVariant::Invalid, sql, params);
}

int DaoTemplate::selectString(QString *str, const QString &sql, const QMap<QString, QVariant> &params) {
    return selectOneOfType(str, QVariant::String, sql, params);
}

int DaoTemplate::selectStrings(QList<QString> *strs, const QString &sql, const QMap<QString, QVariant> &params) {
    QSqlQuery query(DBUtil::getDatabase());
    query.prepare(sql);
    bindValues(&query, params);

    if (query.exec()) {
        while (query.next()) {
            strs->append(query.value(0).toString());
        }

        return 0;
    } else {
        return 1;
    }
}

int DaoTemplate::selectDate(QDate *date,
                            const QString &sql,
                            const QMap<QString, QVariant> &params) {
    return selectOneOfType(date, QVariant::Date, sql, params);
}

int DaoTemplate::selectDateTime(QDateTime *dateTime,
                                const QString &sql,
                                const QMap<QString, QVariant> &params) {
    return selectOneOfType(dateTime, QVariant::DateTime, sql, params);
}

int DaoTemplate::selectOneOfType(void *p,
                                 QVariant::Type type,
                                 const QString &sql,
                                 const QMap<QString, QVariant> &params) {
    QSqlQuery query(DBUtil::getDatabase());
    query.prepare(sql);
    bindValues(&query, params);

    if (query.exec() && query.next()) {
        // 处理每一种类型.
        switch (type) {
        case QVariant::Int:      *((int*)p) = query.value(0).toInt(); break;
        case QVariant::LongLong: *((qint64*)p) = query.value(0).toLongLong(); break;
        case QVariant::Double:   *((float*)p) = query.value(0).toDouble(); break;
        case QVariant::String:   *((QString*)p) = query.value(0).toString(); break;
        case QVariant::Date:     *((QDate*)p) = query.value(0).toDate(); break;
        case QVariant::DateTime: *((QDateTime*)p) = query.value(0).toDateTime(); break;
        case QVariant::Invalid:  *((QVariant*)p) = query.value(0); break;
        default: qDebug() << "Unhandled type";
        }

        return 0;
    } else {
        qDebug() << query.lastError().text();
        return 1;
    }
}

void DaoTemplate::bindValues(QSqlQuery *query, const QMap<QString, QVariant> &params) {
    for (QMap<QString, QVariant>::const_iterator i=params.constBegin(); i!=params.constEnd(); ++i) {
        query->bindValue(":" + i.key(), i.value());
    }
}
