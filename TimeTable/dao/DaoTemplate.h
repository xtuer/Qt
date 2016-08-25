#ifndef DAOTEMPLATE_H
#define DAOTEMPLATE_H

#include "../util/DBUtil.h"

#include <QMap>
#include <QList>
#include <QtSql>
#include <QDebug>
#include <QString>
#include <QVariant>

/**
 * 把查询数据库通用的模板方法放在一起，以便被其他类重用.
 * 极大的简化了持久层类的实现，参考 AdminDao.cpp.
 * Note: QVariant支持的类型其实不需要一个一个的都写一个方法，直接用selectVariant，然后从QVariant取出对应的值即可.
 */
class DaoTemplate {
public:
    /**
     * 执行插入语句，执行成功主键保存到 primaryKey.
     * @param sql
     * @param params
     * @param primaryKey
     * @return 如没有错误返回0， 有错误返回1.
     */
    static int insert(const QString &sql,
                      const QMap<QString, QVariant> &params = (QMap<QString,QVariant>()),
                      int *primaryKey = NULL);

    /**
     * 执行更新语句 (update和delete语句都是更新语句).
     * @param sql
     * @param params
     * @return 如没有错误返回0， 有错误返回1.
     */
    static int update(const QString &sql,
                      const QMap<QString, QVariant> &params = (QMap<QString,QVariant>()));

    /**
     * 查询结果是一个整数值，如查询记录的个数，和等.
     * @param result
     * @param sql
     * @param params
     * @return 如没有错误返回0， 有错误返回1.
     */
    static int selectInt(int *result,
                         const QString &sql,
                         const QMap<QString, QVariant> &params = (QMap<QString,QVariant>()));

    /**
     * 查询结果是一个长整数值, 如果返回的是时间戳时很方便.
     * @param result
     * @param sql
     * @param params
     * @return 如没有错误返回0， 有错误返回1.
     */
    static int selectInt64(qint64 *result,
                           const QString &sql,
                           const QMap<QString, QVariant> &params = (QMap<QString,QVariant>()));

    /**
     * 查询结果是一个 QVariant.
     * @param str
     * @param sql
     * @param params
     * @return 如没有错误返回0， 有错误返回1.
     */
    static int selectVariant(QVariant *var,
                             const QString &sql,
                             const QMap<QString, QVariant> &params = (QMap<QString,QVariant>()));

    /**
     * 查询结果是一个字符串.
     * @param str
     * @param sql
     * @param params
     * @return 如没有错误返回0， 有错误返回1.
     */
    static int selectString(QString *str,
                            const QString &sql,
                            const QMap<QString, QVariant> &params = (QMap<QString,QVariant>()));

    /**
     * 查询结果是多个字符串.
     * @param strs
     * @param sql
     * @param params
     * @return 如没有错误返回0， 有错误返回1.
     */
    static int selectStrings(QList<QString> *strs,
                             const QString &sql,
                             const QMap<QString, QVariant> &params = (QMap<QString,QVariant>()));

    /**
     * 查询结果是一个日期类型.
     * @param date
     * @param sql
     * @param params
     * @return 如没有错误返回0， 有错误返回1.
     */
    static int selectDate(QDate *date,
                          const QString &sql,
                          const QMap<QString, QVariant> &params = (QMap<QString,QVariant>()));

    /**
     * 查询结果是一个日期时间类型.
     * @param date
     * @param sql
     * @param params
     * @return 如没有错误返回0， 有错误返回1.
     */
    static int selectDateTime(QDateTime *dateTime,
                              const QString &sql,
                              const QMap<QString, QVariant> &params = (QMap<QString,QVariant>()));

    /**
     * 执行查询语句，查询到一个 domain object.
     * @param obj 查询得到的每行映射到 obj.
     * @param sql
     * @param mapRow 查询得到的每行映射到一个 T 对象的回调函数.
     * @return 如没有错误返回0， 有错误返回1。
     */
    template <typename T>
    static int selectBean(T *bean, const QString &sql, void mapRow(T *obj, QSqlQuery *query)) {
        return selectBean(bean, sql, QMap<QString, QVariant>(), mapRow);
    }

    template <typename T>
    static int selectBean(T *bean,
                          const QString &sql,
                          const QMap<QString, QVariant> &params,
                          void mapRow(T *obj, QSqlQuery *query)) {
        QSqlQuery query(DBUtil::getDatabase());
        query.prepare(sql);
        bindValues(&query, params);

        if (query.exec() && query.next()) {
            mapRow(bean, &query);

            return 0;
        } else {
            qDebug() << query.lastError().text();
            return 1;
        }
    }

    template<typename T>
    static int selectBeans(QList<T*> *beans,
                           const QString &sql,
                           void mapRow(T *obj, QSqlQuery *query)) {
        return selectBeans(beans, sql, QMap<QString, QVariant>(), mapRow);
    }

    /**
     * 执行查询语句，查询到多个 domain object. 每个查询到的对象都是用 new 创建出来的堆上的对象.
     * @param objs 查询得到的每行映射到对象被放入 objs.
     * @param sql
     * @param params
     * @param mapRow 查询得到的每行映射到一个 T 对象的回调函数.
     * @return 如没有错误返回0， 有错误返回1.
     */
    template<typename T>
    static int selectBeans(QList<T*> *beans,
                           const QString &sql,
                           const QMap<QString, QVariant> &params,
                           void mapRow(T *obj, QSqlQuery *query)) {
        QSqlQuery query(DBUtil::getDatabase());
        query.prepare(sql);
        bindValues(&query, params);

        if (query.exec()) {
            while (query.next()) {
                T *o = new T();
                mapRow(o, &query);
                beans->append(o);
            }

            return 0;
        } else {
            qDebug() << query.lastError().text();
            return 1;
        }
    }

private:
    /**
     * 查询得到一条记录，type指定返回值的类型.
     * @param p
     * @param type
     * @param sql
     * @return 如没有错误返回0， 有错误返回1.
     */
    static int selectOneOfType(void *p,
                               QVariant::Type type,
                               const QString &sql,
                               const QMap<QString, QVariant> &params = (QMap<QString,QVariant>()));

    /**
     * 把 map 中的 key 和 value 绑定到 query 里.
     * @param query
     * @param params
     */
    static void bindValues(QSqlQuery *query, const QMap<QString, QVariant> &params);
};

#endif // DAOTEMPLATE_H
