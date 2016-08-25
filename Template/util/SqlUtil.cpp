#include "SqlUtil.h"
#include "util/SettingUtil.h"

#include <QFile>
#include <QXmlInputSource>
#include <QXmlAttributes>
#include <QXmlParseException>
#include <QDebug>

SINGLETON_INITIALIZE(SqlUtil)

const char * const SQL_TAGNAME_SQL     = "sql";
const char * const SQL_TAGNAME_SQLS    = "sqls";
const char * const SQL_TAGNAME_DEFINE  = "define";
const char * const SQL_TAGNAME_INCLUDE = "include";
const char * const SQL_NAMESPACE       = "namespace";
const char * const SQL_ID     = "id";
const char * const SQL_REF_ID = "refid";

struct SqlUtilPrivate {
    QHash<QString, QString> defines;
    QString sqlNamespace;
    QString currentText;
    QString currentSqlId;
    QString currentDefineId;
    QString currentRefId;
};

SqlUtil::SqlUtil() {
    loadSqls();
}

SqlUtil::~SqlUtil() {
}

QString SqlUtil::getSql(const QString &sqlNamespace, const QString &sqlId) {
    QString sql = sqls.value(sqlNamespace + "__" + sqlId);

    if (sql.isEmpty()) {
        qDebug() << QString("Cannot find SQL for %1::%2").arg(sqlNamespace).arg(sqlId);
    }

    return sql;
}

///////////////////////////////////////////////////////////////////////////////
///                                                                          //
///                       下面的是 protected and private 方法                  //
///                                                                          //
///////////////////////////////////////////////////////////////////////////////

bool SqlUtil::startElement(const QString &namespaceURI,
                           const QString &localName,
                           const QString &qName,
                           const QXmlAttributes &attributes) {
    Q_UNUSED(namespaceURI)
    Q_UNUSED(localName)

    // 1. 取得 SQL 得 xml 文档中得 namespace, sql id, refid, include id
    // 2. 如果是 <sql> 标签，清空 currentText
    // 3. 如果是 <define> 标签，清空 currentText
    if (SQL_TAGNAME_SQL == qName) {
        data->currentSqlId = attributes.value(SQL_ID);
        data->currentText = "";
    } else if (SQL_TAGNAME_INCLUDE == qName) {
        data->currentRefId = attributes.value(SQL_REF_ID);
    } else if (SQL_TAGNAME_DEFINE == qName) {
        data->currentDefineId = attributes.value(SQL_ID);
        data->currentText = "";
    } else if (SQL_TAGNAME_SQLS == qName) {
        data->sqlNamespace = attributes.value(SQL_NAMESPACE);
    }

    return true;
}

bool SqlUtil::endElement(const QString &namespaceURI, const QString &localName, const QString &qName) {
    Q_UNUSED(namespaceURI)
    Q_UNUSED(localName)

    // 1. 如果是 <sql> 标签，则插入 sqls
    // 2. 如果是 <include> 标签，则从 defines 里取其内容加入 sql
    // 3. 如果是 <define> 标签，则存入 defines
    if (SQL_TAGNAME_SQL == qName) {
        sqls.insert(buildKey(data->currentSqlId), data->currentText.simplified());
        data->currentText = "";
    } else if (SQL_TAGNAME_INCLUDE == qName) {
        data->currentText += data->defines.value(buildKey(data->currentRefId));
    } else if (SQL_TAGNAME_DEFINE == qName) {
        data->defines.insert(buildKey(data->currentDefineId), data->currentText.simplified());
    }

    return true;
}

bool SqlUtil::characters(const QString &str) {
    data->currentText += str;
    return true;
}

bool SqlUtil::fatalError(const QXmlParseException &exception) {
    qDebug() << QString("Parse error at line %1, column %2, message: %3")
                .arg(exception.lineNumber())
                .arg(exception.columnNumber())
                .arg(exception.message());
    return false;
}

void SqlUtil::loadSqls() {
    data = new SqlUtilPrivate();
    QStringList sqlFileNames = SettingUtil::getInstance().getSqlFileNames();

    foreach (QString fileName, sqlFileNames) {
        qDebug() << QString("加载 SQL 文件 %1").arg(fileName);

        QFile file(fileName);
        QXmlInputSource inputSource(&file);
        QXmlSimpleReader reader;
        reader.setContentHandler(this);
        reader.setErrorHandler(this);
        reader.parse(inputSource);

        data->defines.clear();
    }

    delete data; // 因为加载完 sql 文件后就不需要 data 了.
}

QString SqlUtil::buildKey(const QString &key) {
    return data->sqlNamespace + "__" + key;
}
