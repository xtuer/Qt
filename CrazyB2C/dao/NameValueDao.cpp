#include "NameValueDao.h"
#include "bean/NameValue.h"
#include "db/DBUtil.h"
#include "db/Sqls.h"

static const char * const SQL_NAMESPACE = "NameValue";

QString NameValueDao::findValueByName(const QString name) {
    QString sql = Singleton<Sqls>::getInstance().getSql(SQL_NAMESPACE, "findValueByName");

    QVariantMap params;
    params["name"] = name;

    return DBUtil::selectString(sql, params);
}

int NameValueDao::insert(const NameValue &nameValue) {
    QString sql = Singleton<Sqls>::getInstance().getSql(SQL_NAMESPACE, "insertNameValue");

    QVariantMap params;
    params["name"]  = nameValue.name;
    params["value"] = nameValue.value;

    return DBUtil::insert(sql, params);
}

bool NameValueDao::update(const NameValue &nameValue) {
    QString sql = Singleton<Sqls>::getInstance().getSql(SQL_NAMESPACE, "updateNameValue");

    QVariantMap params;
    params["id"]    = nameValue.id;
    params["name"]  = nameValue.name;
    params["value"] = nameValue.value;

    return DBUtil::update(sql, params);
}

NameValue NameValueDao::mapToNameValue(const QVariantMap &rowMap) {
    NameValue bean;

    bean.id = rowMap.value("id", -1).toInt();
    bean.name = rowMap.value("name", "").toString();
    bean.value = rowMap.value("value", "").toString();

    return bean;
}
