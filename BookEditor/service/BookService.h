#ifndef BOOKSERVICE_H
#define BOOKSERVICE_H

#include <QList>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QStandardItemModel>
#include <QJsonObject>

class Json;
class CodeInfo;

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

    void readBooks(const QString &path); // 读取教材显示到教材目录树中
    void readChapters(const QString &path); // 读取教材的章节目录

    // 保存教材章节内容
    bool saveChapters(const QString &bookCode,
                      const QString &bookSubject,
                      const QString &bookVersion,
                      const QString &bookName,
                      const QString &bookCover,
                      const QDir &bookDir);

    // 保存教材结构
    bool saveBooks(const QDir &bookDir);

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

    /**
     * 当 previous 为 true 时在 current 节点前插入章节，否则在 current 后插入章节
     *
     * @param current
     * @param previous
     */
    void insertChapter(const QModelIndex &current, bool previous);

    /**
     * 在 parent 节点下增加子章节
     *
     * @param parent 当前选中的章节的 index
     */
    void appendChildChapter(const QModelIndex &parent);

    /**
     * 在 parent 下增加所属的知识点节点
     * @param parent
     */
    void appendKp(const QModelIndex &parent, const QString &kpName, const QString &kpCode);

private:
    /**
     * 创建教程的章节目录
     *
     * @param json 教材的 JSON 对象
     * @param currentChapter 当前章节的对象
     * @param parentChapterNameItem 父章节的 item，使用 currentChapter 创建它的子 item
     */
    void createChapters(const Json &json, const QJsonObject &currentChapter, QStandardItem *parentChapterNameItem);

    /**
     * 创建章节目录对应的 JSON 对象
     *
     * @param chapterNameItem
     * @param chapterCodeItem
     * @return
     */
    QJsonObject createChapterJson(QStandardItem *chapterNameItem, QStandardItem *chapterCodeItem);

    QStandardItemModel *booksModel    = 0; // 教材的 model
    QStandardItemModel *chaptersModel = 0; // 章节的 model
};

#endif // BOOKSERVICE_H
