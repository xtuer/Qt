#ifndef SQLUTIL_H
#define SQLUTIL_H

#include "util/Singleton.h"

#include <QHash>
#include <QString>
#include <QXmlDefaultHandler>

struct SqlUtilPrivate;

/**
 * 用于加载 SQL 语句，用法.
 * SqlUtil::getInstance().getSql("User", "selectById");
 */
class SqlUtil : public QXmlDefaultHandler {
    SINGLETON(SqlUtil)

public:
    QString getSql(const QString &sqlNamespace, const QString &sqlId); // 取得 SQL 语句

protected:
    bool startElement(const QString &namespaceURI,
                      const QString &localName,
                      const QString &qName,
                      const QXmlAttributes &attributes);
    bool endElement(const QString &namespaceURI, const QString &localName, const QString &qName);
    bool characters(const QString &str);
    bool fatalError(const QXmlParseException &exception);

private:
    void loadSqls(); // 加载 SQL 语句
    QString buildKey(const QString &key);

    QHash<QString, QString> sqls;
    SqlUtilPrivate *data;
};

#endif // SQLUTIL_H

/**
SQL 文件的定义
1. <sqls> 必须有 namespace
2. [<define>]*: <define> 必须在 <sql> 前定义，必须有 id 属性才有意义，否则不能被引用
3. [<sql>]*: <sql> 必须有 id 属性才有意义，<sql> 里可以用 <include refid="define_id"> 引用 <define> 的内容

SQL 文件定义 Demo:
<sqls namespace="User">
    <define id="fields">id, username, password</define>

    <sql id="selectById">
        SELECT <include refid="fields"/> FROM user WHERE id=%1
    </sql>

    <sql id="selectAll">
        SELECT id, username, password FROM user
    </sql>

    <sql id="insert">
        INSERT INTO user (username, password) VALUES (:username, :password)
    </sql>

    <sql id="update">
        UPDATE user SET username=:username, password=:password WHERE id=:id
    </sql>
</sqls>
*/
