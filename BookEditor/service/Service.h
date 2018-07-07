#ifndef SERVICE_H
#define SERVICE_H

#include <QDir>
#include <QModelIndex>
#include <QStandardItem>
#include <QStandardItemModel>

/**
 * 通用功能的 Servie
 */
class Service {
public:
    /**
     * 给 parent 下的新节点生成有层级关系的编码。
     * 新节点的编码为: parentCode + (子节点最大编码的后 2 位+1)，例如:
     * 01
     *     0101
     *     0102
     *         010201
     *         010202
     * 02
     *     0201
     *     0202
     * @param parent
     * @return 返回有层级关系的编码
     */
    static QString generateHierarchicalCode(QStandardItemModel *model, const QModelIndex &parent, int codeColumn);

    static bool isPhaseIndex(const QModelIndex &index);   // 判断 index 是否阶段对应的 index
    static bool isSubjectIndex(const QModelIndex &index); // 判断 index 是否学科对应的 index
    static bool isVersionIndex(const QModelIndex &index); // 判断 index 是否版本对应的 index
    static bool isBookIndex(const QModelIndex &index);    // 判断 index 是否教材对应的 index
    static bool isKpIndex(const QModelIndex &index);      // 判断 index 是否知识点对应的 index
    static bool isChapterIndex(const QModelIndex &index); // 判断 index 是否章节对应的 index
    static bool isIndexOfType(const QModelIndex &index, const QString &type); // 判断 index 是否 type 指定的类型

    // 创建阶段的 item
    static QStandardItem* createPhaseItem(const QString &name);

    // 创建学科的 item
    static QStandardItem* createSubjectItem(const QString &name, const QString &code = QString());

    // 创建版本的 item
    static QStandardItem* createVersionItem(const QString &name);

    // 创建教材的 item
    static QStandardItem* createBookItem(const QString &name, const QString &code, const QString &cover);

    // 创建章节目录树中使用的知识点 item
    static QList<QStandardItem *> createKpItemForChapter(const QString &name, const QString &code, const QString &subjectCode);

    // 创建知识点树中使用的章节目录 item
    static QList<QStandardItem *> createChapterItemsForKp(const QString &name, const QString &code, const QString &bookCode);

    /**
     * 创建知识点表中一个知识点对应一行的多个 items
     *
     * @param name 知识点
     * @param code 知识点编码
     * @param cognitionMust     认知发展(必修)
     * @param cognitionOptional 认知发展(选修)
     * @param qualityStudy 学业质量参考(学业考)
     * @param qualityLevel 学业质量参考(等级考)
     * @return item 的列表
     */
    static QList<QStandardItem*> createKpItems(const QString &name, const QString &code,
                                               const QString &cognitionMust = "", const QString &cognitionOptional = "",
                                               const QString &qualityStudy = "", const QString &qualityLevel = "");

    // 向 parent 下添加子节点 items，如果 parent 无效，则添加为第一级节点
    static void appendRow(QStandardItemModel *model, const QModelIndex &parent, const QList<QStandardItem*> &items);

    // 教材章节的文件路径
    static QString chapterFilePath(const QDir &bookDir, const QString &bookCode);

    // 学科知识点的文件路径
    static QString kpFilePath(const QDir &kpDir, const QString &subjectCode);
};

#endif // SERVICE_H
