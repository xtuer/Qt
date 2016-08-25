#include "ClassDao.h"
#include "bean/Class.h"
#include "db/DBUtil.h"
#include "db/Sqls.h"

const char * const SQL_NAMESPACE_CLASS = "Class";

QList<Class> ClassDao::selectAllClasses() {
    QString sql = Singleton<Sqls>::getInstance().getSql(SQL_NAMESPACE_CLASS, "selectAllClasses");
    return DBUtil::selectBeans(mapToClass, sql, QVariantMap());
}

QString ClassDao::selectClassName(int classId) {
    QString sql = Singleton<Sqls>::getInstance().getSql(SQL_NAMESPACE_CLASS, "selectClassName");
    QVariantMap params;
    params["classId"] = classId;

    return DBUtil::selectString(sql, params);
}

Class ClassDao::mapToClass(const QVariantMap &rowMap) {
    Class clazz;

    clazz.id = rowMap.value("id").toInt();
    clazz.name = rowMap.value("name").toString();

    return clazz;
}
