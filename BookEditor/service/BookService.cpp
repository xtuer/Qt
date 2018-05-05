#include "BookService.h"
#include "gui/MessageBox.h"
#include "bean/Book.h"
#include "util/Json.h"
#include "util/Util.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QStandardItemModel>

/**
 * 创建 BookService 对象
 * @param booksModel    教材的 model
 * @param chaptersModel 章节的 model
 */
BookService::BookService(QStandardItemModel *booksModel, QStandardItemModel *chaptersModel)
    : booksModel(booksModel), chaptersModel(chaptersModel) {
}

// 判断 index 是否教材对应的 index
bool BookService::isBookIndex(const QModelIndex &index) const {
    if (!index.isValid()) {
        return false;
    }

    return TYPE_BOOK == index.data(ROLE_TYPE).toString();
}

// 判断 index 是否阶段对应的 index
bool BookService::isPhaseIndex(const QModelIndex &index) const {
    if (!index.isValid()) {
        return false;
    }

    return TYPE_PHASE == index.data(ROLE_TYPE).toString();
}

// 判断 index 是否阶段对应的 index
bool BookService::isSubjectIndex(const QModelIndex &index) const {
    if (!index.isValid()) {
        return false;
    }

    return TYPE_SUBJECT == index.data(ROLE_TYPE).toString();
}

// 判断 index 是否版本对应的 index
bool BookService::isVersionIndex(const QModelIndex &index) const {
    if (!index.isValid()) {
        return false;
    }

    return TYPE_VERSION == index.data(ROLE_TYPE).toString();
}

// 打开教材显示到左侧的教材目录树中
void BookService::openBooks(const QString &path) {
    // 1. 创建教学阶段
    // 2. 创建每个教学阶段下的学科
    // 3. 创建每个学科下的版本
    // 4. 创建每个版本下的教材
    // *  每个 item 都设定一个 ROLE_TYPE 的类型数据，用于判断显示对应的右键菜单

    booksModel->removeRows(0, booksModel->rowCount());
    Json json(path, true);

    QJsonArray phases = json.getJsonArray("phases");
    for (QJsonArray::const_iterator iter = phases.begin(); iter != phases.end(); ++iter) {
        // [1] 创建教学阶段的节点
        QJsonObject phase = iter->toObject();
        QString phaseName = json.getString("title", "", phase);
        QStandardItem *phaseItem = new QStandardItem(phaseName);
        phaseItem->setData(TYPE_PHASE, ROLE_TYPE); // 表示教学阶段
        booksModel->appendRow(phaseItem);

        // [2] 创建教学阶段下的学科
        QJsonArray subjects = json.getJsonArray("subjects", phase);
        for (QJsonArray::const_iterator siter = subjects.begin(); siter != subjects.end(); ++siter) {
            // 创建学科
            QJsonObject subject = siter->toObject();
            QString subjectName = json.getString("title", "", subject);
            QStandardItem *subjectItem = new QStandardItem(subjectName);
            subjectItem->setData(TYPE_SUBJECT, ROLE_TYPE); // 表示学科
            phaseItem->appendRow(subjectItem);

            // [3] 创建学科下的版本
            QJsonArray versions = json.getJsonArray("versions", subject);
            for (QJsonArray::const_iterator viter = versions.begin(); viter != versions.end(); ++viter) {
                QJsonObject version = viter->toObject();
                QString versionName = json.getString("title", "", version);
                QStandardItem *versionItem = new QStandardItem(versionName);
                versionItem->setData(TYPE_VERSION, ROLE_TYPE);
                subjectItem->appendRow(versionItem);

                // 4. 创建版本下的教材
                QJsonArray books = json.getJsonArray("books", version);
                for (QJsonArray::const_iterator biter = books.begin(); biter != books.end(); ++biter) {
                    QJsonObject book  = biter->toObject();
                    QString bookName  = json.getString("title", "", book);
                    QString bookCode  = json.getString("code", "", book);
                    QString bookCover = json.getString("cover", "", book);
                    QStandardItem *bookItem = new QStandardItem(bookName);
                    bookItem->setData(bookCode,  ROLE_CODE);  // 教材的编码
                    bookItem->setData(bookCover, ROLE_COVER); // 教材的封面
                    bookItem->setData(TYPE_BOOK, ROLE_TYPE);  // 表示教材节点
                    versionItem->appendRow(bookItem);
                }
            }
        }
    }
}

// 打开教材的章节目录
void BookService::openChapters(const Json &json) {
    chaptersModel->removeRows(0, chaptersModel->rowCount());
    QJsonArray chapters = json.getJsonArray("chapters");

    for (QJsonArray::const_iterator iter = chapters.begin(); iter != chapters.end(); ++iter) {
        QJsonObject chapter = iter->toObject();
        createChapters(json, chapter, NULL);
    }
}

/**
 * 创建教程的章节目录
 * @param json 教材的 JSON 对象
 * @param currentChapter 当前章节的对象
 * @param parentChapterNameItem 父章节的 item，使用 currentChapter 创建它的子 item
 */
void BookService::createChapters(const Json &json,
                                 const QJsonObject &currentChapter,
                                 QStandardItem *parentChapterNameItem) {
    // 注：每个章节目录都包含 2 列：名字和编码
    QString name = json.getString("title", "", currentChapter);
    QString code = json.getString("code", "", currentChapter);
    QList<QStandardItem*> childColumns;

    if (NULL == parentChapterNameItem) {
        // parentItem 为 NULL，表示需要创建树的根节点
        parentChapterNameItem = new QStandardItem(name);
        childColumns << parentChapterNameItem << new QStandardItem(code);
        chaptersModel->appendRow(childColumns);
    } else {
        // parentItem 不为 NULL，表示创建它的子节点
        QStandardItem *currentChapterNameItem = new QStandardItem(name);
        childColumns << currentChapterNameItem << new QStandardItem(code);
        parentChapterNameItem->appendRow(childColumns);
        parentChapterNameItem = currentChapterNameItem;
    }

    // 章节下的章节
    QJsonArray subchapters = json.getJsonArray("children", currentChapter);
    for (QJsonArray::const_iterator iter = subchapters.begin(); iter != subchapters.end(); ++iter) {
        QJsonObject subchapter = iter->toObject();
        createChapters(json, subchapter, parentChapterNameItem);
    }
}

/**
 * 如果教材的编码都是唯一的，则验证通过返回 true，如果编码被重复使用验证不通过返回 false
 *
 * @param currentBookIndex 教材树中当前被选中节点的 index
 * @param editingBookCode  正在编辑的教材编码，在窗口右边
 * @param error 校验不通过的错误信息，不能为 NULL
 * @return 校验通过返回 true，不通过返回 false
 */
bool BookService::validateBooks(const QModelIndex &currentBookIndex,
                                const QString &editingBookCode,
                                QString *error) const {
    QList<CodeInfo> booksInfo;

    // 逐层遍历访问得到教材信息，保存到数组 booksInfo 中
    for (int i = 0; i < booksModel->rowCount(); ++i) {
        // 阶段
        QStandardItem *phaseItem = booksModel->item(i, 0);
        QString phaseName = phaseItem->data(Qt::DisplayRole).toString();

        if (phaseName.trimmed().isEmpty()) {
            error->append("阶段不能为空\n");
            return false;
        }

        for (int j = 0; j < phaseItem->rowCount(); ++j) {
            // 学科
            QStandardItem *subjectItem = phaseItem->child(j, 0);
            QString subjectName = subjectItem->data(Qt::DisplayRole).toString();

            if (subjectName.trimmed().isEmpty()) {
                error->append("学科不能为空\n");
                return false;
            }

            for (int k = 0; k < subjectItem->rowCount(); ++k) {
                // 版本
                QStandardItem *versionItem = subjectItem->child(k, 0);
                QString versionName = versionItem->data(Qt::DisplayRole).toString();

                if (versionName.trimmed().isEmpty()) {
                    error->append("版本不能为空\n");
                    return false;
                }

                for (int m = 0; m < versionItem->rowCount(); ++m) {
                    // 教材
                    QStandardItem *bookItem = versionItem->child(m, 0);
                    QString bookCode    = bookItem->data(ROLE_CODE).toString();
                    QString bookVersion = bookItem->data(Qt::DisplayRole).toString();
                    QString bookInfo    = QString("%1 > %2 > %3 > %4")
                            .arg(phaseName).arg(subjectName)
                            .arg(versionName).arg(bookVersion);

                    if (bookVersion.trimmed().isEmpty()) {
                        error->append("教材名字不能为空\n");
                        return false;
                    }

                    // 选中的教材的 code 更新为编辑的教材 code，这样就能够判断当前编辑的 code 是否可用
                    if (currentBookIndex.isValid() && booksModel->itemFromIndex(currentBookIndex) == bookItem) {
                        bookCode = editingBookCode.trimmed();
                    }

                    booksInfo.append(CodeInfo(bookCode, bookInfo));
                }
            }
        }
    }

    return validateCodeInfos(&booksInfo, error);
}

/**
 * 如果当前教材的章节编码都是唯一的，则验证通过返回 true，如果编码被重复使用验证不通过返回 false
 *
 * @param error 校验不通过的错误信息，不能为 NULL
 * @return 校验通过返回 true，不通过返回 false
 */
bool BookService::validateChapters(QString *error) const {
    QList<CodeInfo> chaptersInfo;

    for (int i = 0; i < chaptersModel->rowCount(); ++i) {
        QStandardItem *chapterNameItem = chaptersModel->item(i, 0);
        QStandardItem *chapterCodeItem = chaptersModel->item(i, 1);

        travelChapter(chapterNameItem, chapterCodeItem, &chaptersInfo);
    }

    return validateCodeInfos(&chaptersInfo, error);
}

/**
 * 校验 CodeInfo 数组中的 code 是否被重复使用，如果有重复使用时返回 false 并且 error 中记录重复使用的 code:info 数据
 *
 * @param codeInfos CodeInfo 数组
 * @param error 校验不通过的错误信息，不能为 NULL
 * @return 校验通过返回 true，不通过返回 false
 */
bool BookService::validateCodeInfos(QList<CodeInfo> *codeInfos, QString *error) const {
    // 1. 先排序：先根据 code 排序，code 相同再根据 info 排序
    // 2. 对有序数组进行扫描，找出重复的数据

    // [1] 先排序：先根据 code 排序，code 相同再根据 info 排序
    std::sort(codeInfos->begin(), codeInfos->end(), [](const CodeInfo & a, const CodeInfo & b) -> bool {
        if (a.code == b.code) {
            return a.info < b.info;
        } else {
            return a.code < b.code;
        }
    });

    // 打印出所有的 CodeInfo 信息，便于调试查看
    for (const CodeInfo &ci : *codeInfos) {
        qDebug().noquote() << QString("%1 - %2").arg(ci.code).arg(ci.info);
    }

    // [2] 对有序数组进行扫描，找出重复的数据
    // 逻辑:
    //     1. j 初始化为 i+1，从 i 下一个开始进行比较
    //     2. 当第 j 个和第 i 个相同时，记录数据，继续 j 所在的循环
    //     3. 当第 j 行和第 i 个不同时，结束 j 所在循环，并赋值 i 为 j，进行 i 所在的循环
    bool ok = true;
    int length = codeInfos->size();
    for (int i = 0, j = 0; i < length; i = j) {
        bool    first = true;
        QString firstCode = codeInfos->at(i).code;
        QString firstInfo = codeInfos->at(i).info;

        for (j = i+1; j < codeInfos->size(); ++j) {
            QString currentCode = codeInfos->at(j).code;
            QString currentInfo = codeInfos->at(j).info;

            if (currentCode != firstCode) {
                // 找到不同数据，结束循环
                break;
            } else {
                // 找到相同数据，记录重复数据信息
                ok = false;

                if (first) {
                    first = false;
                    error->append(QString("%1 - %2\n").arg(firstCode).arg(firstInfo));
                }

                error->append(QString("%1 - %2\n").arg(currentCode).arg(currentInfo));
            }
        }
    }

    return ok;
}

/**
 * 遍历教材的章节目录，把所有的章节信息保存到 chaptersInfo
 *
 * @param chapterNameItem 章节名字的 item
 * @param chapterCodeItem 章节编码的 item
 * @param chaptersInfo 保存章节信息的数组
 */
void BookService::travelChapter(QStandardItem *chapterNameItem,
                                QStandardItem *chapterCodeItem,
                                QList<CodeInfo> *chaptersInfo) const {
    if (NULL == chapterNameItem || NULL == chapterCodeItem) { return; }
    QString chapterName = chapterNameItem->data(Qt::DisplayRole).toString();
    QString chapterCode = chapterCodeItem->data(Qt::DisplayRole).toString();
    chaptersInfo->append(CodeInfo(chapterCode, chapterName));

    // 递归遍历子章节
    for (int i = 0; i < chapterNameItem->rowCount(); ++i) {
        QStandardItem *childChapterNameItem = chapterNameItem->child(i, 0);
        QStandardItem *childChapterCodeItem = chapterNameItem->child(i, 1);
        travelChapter(childChapterNameItem, childChapterCodeItem, chaptersInfo);
    }
}

bool BookService::saveBook(const QString &bookCode,
                           const QString &bookSubject,
                           const QString &bookVersion,
                           const QString &bookName,
                           const QString &bookCover,
                           const QDir &bookDir) {
    QJsonArray chaptersJson;
    for (int i = 0; i < chaptersModel->rowCount(); ++i) {
        QStandardItem *chapterNameItem = chaptersModel->item(i, 0);
        QStandardItem *chapterCodeItem = chaptersModel->item(i, 1);

        chaptersJson.append(createChapterJson(chapterNameItem, chapterCodeItem));
    }

    QJsonObject book;
    book.insert("code",     bookCode);
    book.insert("subject",  bookSubject);
    book.insert("version",  bookVersion);
    book.insert("title",     bookName);
    book.insert("cover",    bookCover);
    book.insert("chapters", chaptersJson);

    // 保存到文件
    return Util::writeStringToFile(QJsonDocument(book).toJson(), bookDir.filePath(bookCode + ".json"));
}

bool BookService::saveBooks(const QDir &bookDir) {
    QJsonArray phases;

    // 逐层遍历访问得到教材信息，保存到数组 booksInfo 中
    for (int i = 0; i < booksModel->rowCount(); ++i) {
        // 阶段
        QStandardItem *phaseItem = booksModel->item(i, 0);
        QString phaseName = phaseItem->data(Qt::DisplayRole).toString();

        QJsonArray subjects;
        for (int j = 0; j < phaseItem->rowCount(); ++j) {
            // 学科
            QStandardItem *subjectItem = phaseItem->child(j, 0);
            QString subjectName = subjectItem->data(Qt::DisplayRole).toString();

            QJsonArray versions;
            for (int k = 0; k < subjectItem->rowCount(); ++k) {
                // 版本
                QStandardItem *versionItem = subjectItem->child(k, 0);
                QString versionName =  versionItem->data(Qt::DisplayRole).toString();

                QJsonArray books;
                for (int m = 0; m < versionItem->rowCount(); ++m) {
                    // 教材
                    QStandardItem *bookItem = versionItem->child(m, 0);
                    QString bookName  = bookItem->data(Qt::DisplayRole).toString();
                    QString bookCode  = bookItem->data(ROLE_CODE).toString();
                    QString bookCover = bookItem->data(ROLE_COVER).toString();

                    QJsonObject book;
                    book.insert("title", bookName);
                    book.insert("code",  bookCode);
                    book.insert("cover", bookCover);
                    books.append(book);
                }

                QJsonObject version;
                version.insert("title", versionName);
                version.insert("books", books);
                versions.append(version);
            }

            QJsonObject subject;
            subject.insert("title", subjectName);
            subject.insert("versions", versions);
            subjects.append(subject);
        }

        QJsonObject phase;
        phase.insert("title", phaseName);
        phase.insert("subjects", subjects);
        phases.append(phase);
    }

    QJsonObject root;
    root.insert("phases", phases);

    // 保存到文件
    return Util::writeStringToFile(QJsonDocument(root).toJson(), bookDir.filePath("books.json"));
}

QJsonObject BookService::createChapterJson(QStandardItem *chapterNameItem, QStandardItem *chapterCodeItem) {
    if (NULL == chapterNameItem || NULL == chapterCodeItem) { return QJsonObject(); }

    QString chapterName = chapterNameItem->data(Qt::DisplayRole).toString();
    QString chapterCode = chapterCodeItem->data(Qt::DisplayRole).toString();

    // 递归遍历子章节
    QJsonArray childrenChapters;
    for (int i = 0; i < chapterNameItem->rowCount(); ++i) {
        QStandardItem *childChapterNameItem = chapterNameItem->child(i, 0);
        QStandardItem *childChapterCodeItem = chapterNameItem->child(i, 1);
        childrenChapters.append(createChapterJson(childChapterNameItem, childChapterCodeItem));
    }

    QJsonObject chapterJson;
    chapterJson.insert("title", chapterName);
    chapterJson.insert("code", chapterCode);
    chapterJson.insert("children", childrenChapters);

    return chapterJson;

}

