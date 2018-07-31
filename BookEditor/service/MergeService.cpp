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

// 递归遍历章节: 把所有章节所属知识点的关系保存到 QList kpsOfChapters 中
void MergeService::travelChapters(const Json &json, const QJsonObject &chapter) {
    QString chapterCode = json.getString("code", "",  chapter);
    QString chapterName = json.getString("title", "", chapter);

    // 章节所属知识点
    QJsonArray kps = json.getJsonArray("kps", chapter);
    for (QJsonArray::const_iterator iter = kps.begin(); iter != kps.end(); ++iter) {
        QJsonObject kp = iter->toObject();
        QString kpCode = json.getString("code", "", kp);
        QString kpName = json.getString("title", "", kp);
        QString kpSubjectCode = json.getString("subjectCode", "", kp);

        kpsOfChapters.append({ kpSubjectCode, kpCode, kpName, bookCode, chapterCode, chapterName });
    }

    // 递归遍历章节的子章节
    QJsonArray subchapters = json.getJsonArray("children", chapter);
    for (QJsonArray::const_iterator iter = subchapters.begin(); iter != subchapters.end(); ++iter) {
        QJsonObject subchapter = iter->toObject();
        travelChapters(json, subchapter);
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

// 递归遍历知识点
void MergeService::travelKps(QJsonObject &kp, const QString &subjectCode) {
    // 1. 先把知识点的所有子知识点添加到 newChildren，删除章节节点
    // 2. 遍历章节所属知识点关系，如果找到了当前知识点则创建一个章节节点添加到 newChildren 里
    //    (存在判断条件为: 章节所属知识点关系中找到当前学科编码和知识点编码相等的数据)
    // 3. 把 newChildren 重新设置回 kp 的 children，因为操作 Qt 中 JSON 操作的都是对象的副本
    QString code = kp.value("code").toString();

    QJsonArray newChildren;
    QJsonArray children = kp.value("children").toArray();
    QJsonArray chapters; // 章节

    // [1] 先把知识点的所有子知识点添加到 newChildren，删除章节节点
    for (QJsonArray::const_iterator iter = children.begin(); iter != children.end(); ++iter) {
        QJsonObject child = iter->toObject();
        travelKps(child, subjectCode);

        // 先删除知识点所属章节，在后面再加入
        if (!child.value("chapter").toBool()) {
            newChildren.append(child);
        }
    }

    // 到此 newChildren 中的为已有的节点数据
    // [2] 遍历章节的知识点关系，如果找到了当前知识点则创建一个章节节点添加到 chapters 里
    for (auto kc : kpsOfChapters) {
        QString subjectCodeX     = kc.value(0);
        QString kpCodeX          = kc.value(1);
        QString chapterBookCodeX = kc.value(3);
        QString chapterCodeX     = kc.value(4);
        QString chapterNameX     = kc.value(5);

        // 存在判断条件为: 章节所属知识点关系中找到当前学科编码和知识点编码相等的数据
        if (subjectCodeX == subjectCode && kpCodeX == code) {
            QJsonObject chapter;
            chapter["code"]     = chapterCodeX;
            chapter["title"]    = chapterNameX;
            chapter["bookCode"] = chapterBookCodeX;

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
                chapters.append(chapter);
            }
        }
    }

    // [3] 把 newChildren 重新设置回 kp 的 children，因为操作 Qt 中 JSON 操作的都是对象的副本
    if (newChildren.size() > 0) {
        kp["children"] = newChildren;
    } else {
        kp.remove("children"); // 删除空的 children 是为了让生成的 JSON 文件更干净
    }

    if (chapters.size() > 0) {
        kp["chapters"] = chapters;
    } else {
        kp.remove("chapters");
    }
}
