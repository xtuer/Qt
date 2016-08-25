#include "ClazzDao.h"
#include "bean/Clazz.h"
#include "dao/DaoTemplate.h"
#include "util/DBUtil.h"
#include "util/SqlUtil.h"

int ClazzDao::findById(int id, Clazz* clazz) {
    QString sql = SqlUtil::sql("findClazzById").arg(id);
    return DaoTemplate::selectBean(clazz, sql, mapRow);
}

int ClazzDao::findAllClazzes(QList<Clazz*> *clazzes) {
    QString sql = SqlUtil::sql("findAllClazzes");
    return DaoTemplate::selectBeans(clazzes, sql, mapRow);
}

void ClazzDao::mapRow(Clazz* clazz, QSqlQuery* query) {
    clazz->id = query->value("id").toInt();
    clazz->name = query->value("name").toString();
}
