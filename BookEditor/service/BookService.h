#ifndef BOOKSERVICE_H
#define BOOKSERVICE_H

#include <QList>

class QDir;
class CodeInfo;
class Json;
class QJsonObject;
class QString;
class QStandardItem;
class QModelIndex;
class QStandardItemModel;

/**
 * 提供教材需要的服务功能，例如判断树节点的类型，打开教材的描述文件创建教材、创建章节的树，
 * 验证教材和章节是否有效等。
 */
class BookService {
public:
    /**
     * 创建 BookService 对象
     * @param booksModel    教材的 model
     * @param chaptersModel 章节的 model
     */
    BookService(QStandardItemModel *booksModel, QStandardItemModel *chaptersModel);

    bool isBookIndex(const QModelIndex &index) const;    // 判断 index 是否教材对应的 index
    bool isPhaseIndex(const QModelIndex &index) const;   // 判断 index 是否阶段对应的 index
    bool isSubjectIndex(const QModelIndex &index) const; // 判断 index 是否阶段对应的 index

    void openBooks(const QString &path); // 打开教材显示到教材目录树中
    void openChapters(const Json &json); // 打开教材的章节目录

    /**
     * 如果教材的编码都是唯一的，则验证通过返回 true，如果编码被重复使用验证不通过返回 false
     *
     * @param currentBookIndex 教材树中当前被选中节点的 index
     * @param editingBookCode  正在编辑的教材编码，在窗口右边
     * @param error 校验不通过的错误信息，不能为 NULL
     * @return 校验通过返回 true，不通过返回 false
     */
    bool validateBooks(const QModelIndex &currentBookIndex, const QString &editingBookCode, QString *error) const;

    /**
     * 如果当前教材的章节编码都是唯一的，则验证通过返回 true，如果编码被重复使用验证不通过返回 false
     *
     * @param error 校验不通过的错误信息，不能为 NULL
     * @return 校验通过返回 true，不通过返回 false
     */
    bool validateChapters(QString *error) const;

    // 保存教材内容
    bool saveBook(const QString &bookCode,
                  const QString &bookSubject,
                  const QString &bookVersion,
                  const QString &bookRequirement,
                  const QString &bookCover,
                  const QDir &bookDir);
    bool saveBooks(const QDir &bookDir); // 保存教材结构
    QJsonObject createChapterJson(QStandardItem *chapterNameItem, QStandardItem *chapterCodeItem);

private:
    /**
     * 创建教程的章节目录
     * @param json 教材的 JSON 对象
     * @param currentChapter 当前章节的对象
     * @param parentChapterNameItem 父章节的 item，使用 currentChapter 创建它的子 item
     */
    void createChapters(const Json &json,
                        const QJsonObject &currentChapter,
                        QStandardItem *parentChapterNameItem);

    /**
     * 校验 CodeInfo 数组中的 code 是否被重复使用，如果有重复使用时返回 false 并且 error 中记录重复使用的 code:info 数据
     *
     * @param codeInfos CodeInfo 数组
     * @param error 校验不通过的错误信息，不能为 NULL
     * @return 校验通过返回 true，不通过返回 false
     */
    bool validateCodeInfos(QList<CodeInfo> *codeInfos, QString *error) const;

    /**
     * 遍历教材的章节目录，把所有的章节信息保存到 chaptersInfo
     *
     * @param chapterNameItem 章节名字的 item
     * @param chapterCodeItem 章节编码的 item
     * @param chaptersInfo 保存章节信息的数组
     */
    void travelChapter(QStandardItem *chapterNameItem,
                       QStandardItem *chapterCodeItem,
                       QList<CodeInfo> *chaptersInfo) const;

    QStandardItemModel *booksModel    = 0; // 教材的 model
    QStandardItemModel *chaptersModel = 0; // 章节的 model
};

#endif // BOOKSERVICE_H
