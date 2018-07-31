#ifndef MERGESERVICE_H
#define MERGESERVICE_H

#include <QList>
#include <QStringList>
#include <QJsonObject>

class Json;

/**
 * 把章节目录中章节所属知识点合并到知识点对应的文件中
 */
class MergeService {
public:
    MergeService(const QString &booksDir = "data/books", const QString &kpsDir = "data/kps");

    /**
     * 获取章节和它所属的知识点关系列表
     * 0: 知识点学科编码
     * 1: 知识点编码
     * 2: 知识点名字
     * 3: 教材编码
     * 4: 章节编码
     * 5: 章节名字
     *
     * @return 返回章节和它所属的知识点关系列表
     */
    QList<QStringList> getKpsOfChapters() const;

private:
    QString bookCode; // 教材编码
    QString booksDir; // 教材所在文件夹
    QString kpsDir;   // 知识点所在文件夹

    // 章节和它所属的知识点关系列表
    // 0: 知识点学科编码
    // 1: 知识点编码
    // 2: 知识点名字
    // 3: 教材编码
    // 4: 章节编码
    // 5: 章节名字
    QList<QStringList> kpsOfChapters;

    // 遍历 data/books 下的所有章节对应的文件，得到章节和它所属的知识点关系列表
    void loadKpsOfChapters();

    // 合并章节目录到知识点下
    void mergeChaptersToKps();

    // 递归遍历章节: 把所有章节所属知识点的关系保存到 QList kpsOfChapters 中
    void travelChapters(const Json &json, const QJsonObject &chapter);

    // 遍历知识点
    void travelKps(QJsonObject &kp, const QString &subjectCode);
};

#endif // MERGESERVICE_H
