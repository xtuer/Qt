#include "MergeService.h"
#include "util/Json.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDirIterator>

MergeService::MergeService(const QString &booksDir, const QString &kpsDir)
    : booksDir(booksDir), kpsDir(kpsDir) {
    loadKpsOfChapters();
    mergeChaptersToKps();
}

QList<QStringList> MergeService::getKpsOfChapters() const {
    return kpsOfChapters;
}

// 遍历 data/books 下的所有章节对应的文件，得到章节和它所属的知识点关系列表
void MergeService::loadKpsOfChapters() {
    QDirIterator it(booksDir, QDir::NoDotAndDotDot | QDir::Files);

    while (it.hasNext()) {
        QString path = it.next();

        if (path.endsWith("books.json") || !path.endsWith("json")) {
            continue;
        }

        Json json(path, true);
        bookCode = json.getString("code"); // 教材编码
        QJsonArray chapters = json.getJsonArray("chapters");

        for (QJsonArray::const_iterator iter = chapters.begin(); iter != chapters.end(); ++iter) {
            QJsonObject chapter = iter->toObject();
            travelChapters(json, chapter);
        }
    }
}

// 合并章节目录到知识点下
void MergeService::mergeChaptersToKps() {
    QDirIterator it(kpsDir, QDir::NoDotAndDotDot | QDir::Files);

    while (it.hasNext()) {
        QString path = it.next();

        if (path.endsWith("kps.json") || !path.endsWith("json")) {
            continue;
        }

        // 读取 JSON 文件
        QByteArray json;
        {
            QFile inFile(path);
            if (!inFile.open(QIODevice::ReadOnly)) {
                continue;
            }
            json = inFile.readAll();
            inFile.close();
        }

        QJsonDocument jsonDocument = QJsonDocument::fromJson(json);
        QJsonObject root   = jsonDocument.object();
        QString subjectCoe = root.value("subjectCode").toString();
        QJsonArray kps     = root.value("kps").toArray();
        QJsonArray newKps;

        for (QJsonArray::const_iterator iter = kps.begin(); iter != kps.end(); ++iter) {
            QJsonObject kp = iter->toObject();
            travelKps(kp, subjectCoe);
            newKps.append(kp);
        }
        root["kps"] = newKps;

        QFile outFile(path);

        if (!outFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            continue;
        }

        QTextStream out(&outFile);
        out << QJsonDocument(root).toJson(QJsonDocument::Indented);
        out.flush();
        outFile.close();
    }
}

// 遍历章节
void MergeService::travelChapters(const Json &json, const QJsonObject &chapter) {
    QString chapterCode = json.getString("code", "",  chapter);
    QString chapterName = json.getString("title", "", chapter);

    // 章节的子章节
    QJsonArray subchapters = json.getJsonArray("children", chapter);
    for (QJsonArray::const_iterator iter = subchapters.begin(); iter != subchapters.end(); ++iter) {
        QJsonObject subchapter = iter->toObject();

        // 如果 kpCode 和 kpSubjectCode 不为空，则是章节所属知识点
        bool kp = json.getBool("kp", false, subchapter);

        if (kp) {
            QString kpCode = json.getString("kpCode", "", subchapter);
            QString kpName = json.getString("kpName", "", subchapter);
            QString kpSubjectCode = json.getString("kpSubjectCode", "", subchapter);

            // 知识点学科编码, 知识点编码, 知识点名字, 教材编码, 章节编码, 章节名字
            kpsOfChapters.append({ kpSubjectCode, kpCode, kpName, bookCode, chapterCode, chapterName });
        }

        travelChapters(json, subchapter);
    }
}

// 遍历知识点
void MergeService::travelKps(QJsonObject &kp, const QString &subjectCode) {
    QString code = kp.value("code").toString();

    QJsonArray newChildren;
    QJsonArray children = kp.value("children").toArray();

    for (QJsonArray::const_iterator iter = children.begin(); iter != children.end(); ++iter) {
        QJsonObject child = iter->toObject();
        travelKps(child, subjectCode);

        // 先删除知识点所属章节，在后面再加入
        if (!child.value("chapter").toBool()) {
            newChildren.append(child);
        }
    }

    // 到此 newChildren 中的为已有的节点数据
    // 在章节的知识点里搜索当前知识点是否存在，存在判断条件为学科编码和知识点编码相等
    for (auto kc : kpsOfChapters) {
        // 学科编码和知识点编码相等
        QString subjectCodeX     = kc.value(0);
        QString kpCodeX          = kc.value(1);
        QString chapterBookCodeX = kc.value(3);
        QString chapterCodeX     = kc.value(4);
        QString chapterNameX     = kc.value(5);

        if (subjectCodeX == subjectCode && kpCodeX == code) {
            QJsonObject chapter;
            chapter["chapter"]         = true;
            chapter["chapterCode"]     = chapterCodeX;
            chapter["chapterName"]     = chapterNameX;
            chapter["chapterBookCode"] = chapterBookCodeX;
            chapter["title"]           = chapterNameX;
            chapter["code"]            = "";

            // 不在 newChildren 里才添加，避免重复添加
            bool exist = false;
            for (QJsonArray::const_iterator iter = newChildren.begin(); iter != newChildren.end(); ++iter) {
                QJsonObject child = iter->toObject();
                QString existingChapterCode     = child.value("chapterCode").toString();
                QString existingChapterBookCode = child.value("chapterBookCode").toString();

                if (existingChapterCode == chapterCodeX && existingChapterBookCode == chapterBookCodeX) {
                    exist = true;
                }
            }

            if (!exist) {
                newChildren.append(chapter);
            }
        }
    }

    if (newChildren.size() > 0) {
        kp["children"] = newChildren;
    } else {
        kp.remove("children");
    }
}
