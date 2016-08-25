#include "SqlUtil.h"
#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QDebug>

SINGLETON_INITIALATE(SqlUtil)

SqlUtil::SqlUtil() {
    loadXml();
    loadPrefixes();
    loadSqls();
}

SqlUtil::~SqlUtil() {
}

QString SqlUtil::sql(const QString &name) {
    return getInstance().sqls.value(name, "");
}

void SqlUtil::loadXml() {
    QFile file("sqls.xml");

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << __FILE__ << " : " << __LINE__ << " file.open() error";
        exit(2);
    }

    if (!doc.setContent(&file)) {
        file.close();
        qDebug() << __FILE__ << " : " << __LINE__ << " doc.setContent() error";
        exit(3);
    }

    file.close();
}

void SqlUtil::loadPrefixes() {
    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();

    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull() && e.tagName() == "prefix" && e.hasAttribute("name")) {
            prefixes.insert(e.attribute("name"), e.text().simplified());
        }
        n = n.nextSibling();
    }
}

void SqlUtil::loadSqls() {
    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();

    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull() && e.tagName() == "sql" && e.hasAttribute("name")) {
            if (e.hasAttribute("prefix")) {
                QString prefix = prefixes.value(e.attribute("prefix").trimmed());
                sqls.insert(e.attribute("name"), prefix + " " + e.text().simplified());
            } else {
                sqls.insert(e.attribute("name"), e.text().simplified());
            }
        }
        n = n.nextSibling();
    }
}
