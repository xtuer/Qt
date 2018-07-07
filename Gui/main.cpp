#include <QApplication>
#include <QDebug>
#include <QModelIndexList>
#include "Form.h"
#include <QDir>
#include <QFile>
#include <QDirIterator>
#include "Json.h"
#include <QList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextStream>

void readBook(const QString &path, QList<QStringList> *list);
void readChapters(const Json &json, const QJsonObject &chapter, const QString &bookCode, QList<QStringList> *list);
void readKp(const QString &path, const QList<QStringList> &list);
void readKps(QJsonObject &kp, const QString &subjectCode, const QList<QStringList> &list);

int main(int argc, char *argv[]) {
    Q_UNUSED(argc)
    Q_UNUSED(argv)

    QDirIterator it("/Users/Biao/Temp/Qt/BookEditor/bin/data/books", QDir::NoDotAndDotDot | QDir::Files);
    QList<QStringList> list;

    while (it.hasNext()) {
        QString path = it.next();
        if (path.endsWith("books.json") || !path.endsWith("json")) {
            continue;
        }

        readBook(path, &list);
    }

    for (auto l : list) {
        qDebug() << l;
    }

    readKp("data/kps/cz_sx.json", list);

    return 0;
}

void readBook(const QString &path, QList<QStringList> *list) {
    Json json(path, true);
    QString code = json.getString("code");
    qDebug() << code;

    QJsonArray chapters = json.getJsonArray("chapters");

    for (QJsonArray::const_iterator iter = chapters.begin(); iter != chapters.end(); ++iter) {
        QJsonObject chapter = iter->toObject();
        readChapters(json, chapter, code, list);
    }
}

void readChapters(const Json &json, const QJsonObject &chapter, const QString &bookCode, QList<QStringList> *list) {
    QString chapterCode = json.getString("code", "", chapter);
    QString chapterName = json.getString("title", "", chapter);

    // 章节下的章节
    QJsonArray subchapters = json.getJsonArray("children", chapter);
    for (QJsonArray::const_iterator iter = subchapters.begin(); iter != subchapters.end(); ++iter) {
        QJsonObject subchapter = iter->toObject();

        // 如果 kpCode 和 kpSubjectCode 不为空，则是章节所属知识点
        QString kpCode = json.getString("kpCode", "", subchapter);
        QString kpName = json.getString("kpName", "", subchapter);
        QString kpSubjectCode = json.getString("kpSubjectCode", "", subchapter);

        if (!kpCode.isEmpty() && !kpSubjectCode.isEmpty()) {
            // 知识点学科编码::知识点编码::知识点名字::教材编码::章节编码::章节名字
//            qDebug().noquote() << QString("%1::%2::%3::%4::%5::%6")
//                                  .arg(kpSubjectCode).arg(kpCode).arg(kpName)
//                                  .arg(bookCode).arg(chapterCode).arg(chapterName);
            list->append({ kpSubjectCode, kpCode, kpName, bookCode, chapterCode, chapterName });
        }

        readChapters(json, subchapter, bookCode, list);
    }
}

void readKp(const QString &path, const QList<QStringList> &list) {
    QFile file(path);
    file.open(QIODevice::ReadOnly);

    QJsonDocument jsonDocument = QJsonDocument::fromJson(file.readAll());
    QJsonObject root = jsonDocument.object();
    QString subjectCoe = root.value("code").toString();
    QJsonArray kps = root.value("kps").toArray();
    QJsonArray newKps;
    file.close();

    for (QJsonArray::const_iterator iter = kps.begin(); iter != kps.end(); ++iter) {
        QJsonObject kp = iter->toObject();
        readKps(kp, subjectCoe, list);
        newKps.append(kp);
    }
    root["kps"] = newKps;

    // 保存
    QFile file2("/Users/Biao/Desktop/r.json");
    if (!file2.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        return;
    }
    QTextStream out(&file2);
    out << QJsonDocument(root).toJson(QJsonDocument::Indented);
    out.flush();
    file2.close();
}

void readKps(QJsonObject &kp, const QString &subjectCode, const QList<QStringList> &list) {
    QString name = kp.value("title").toString();
    QString code = kp.value("code").toString();

    qDebug() << subjectCode << name << code;

    QJsonArray newChildren;
    QJsonArray children = kp.value("children").toArray();

    for (QJsonArray::const_iterator iter = children.begin(); iter != children.end(); ++iter) {
        QJsonObject child = iter->toObject();
        readKps(child, subjectCode, list);
        newChildren.append(child);
    }

    // set up
    for (auto l : list) {
        if (l.value(0) == subjectCode && l.value(1) == code) {
            QJsonObject chapter;
            chapter["chapter"] = true;
            chapter["chapterBookCode"] = l.value(3);
            chapter["chapterCode"] = l.value(4);
            chapter["chapterName"] = l.value(5);
            chapter["title"] = l.value(5);
            newChildren.append(chapter);
            qDebug() << "====> " << chapter;
        }
    }

    kp["children"] = newChildren;
}
