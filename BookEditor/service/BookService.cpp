#include "BookService.h"
#include "CodeInfoService.h"
#include "Service.h"
#include "bean/Book.h"
#include "util/Json.h"
#include "util/Util.h"

/**
 * 创建 BookService 对象
 * @param booksModel    教材的 model
 * @param chaptersModel 章节的 model
 */
BookService::BookService(QStandardItemModel *booksModel, QStandardItemModel *chaptersModel, const QDir &booksDir)
    : booksDir(booksDir), booksModel(booksModel), chaptersModel(chaptersModel) {
}

// 读取教材显示到左侧的教材目录树中
void BookService::readBooks() {
    // 1. 创建教学阶段
    // 2. 创建每个教学阶段下的学科
    // 3. 创建每个学科下的版本
    // 4. 创建每个版本下的教材
    // *  每个 item 都设定一个 ROLE_TYPE 的类型数据，用于判断显示对应的右键菜单

    QFileInfo info = QFileInfo(Service::booksFilePath(booksDir));

    if (!info.exists()) {
        return;
    }

    booksModel->removeRows(0, booksModel->rowCount());
    Json json(info.absoluteFilePath(), true);

    QJsonArray phases = json.getJsonArray("phases");
    for (QJsonArray::const_iterator piter = phases.begin(); piter != phases.end(); ++piter) {
        // [1] 创建教学阶段的节点
        QJsonObject phase = piter->toObject();
        QString phaseName = json.getString("title", "", phase);
        QStandardItem *phaseItem = Service::createPhaseItem(phaseName);
        booksModel->appendRow(phaseItem);

        // [2] 创建教学阶段下的学科
        QJsonArray subjects = json.getJsonArray("subjects", phase);
        for (QJsonArray::const_iterator siter = subjects.begin(); siter != subjects.end(); ++siter) {
            // 创建学科
            QJsonObject subject = siter->toObject();
            QString subjectName = json.getString("title", "", subject);
            QStandardItem *subjectItem = Service::createSubjectItem(subjectName);
            phaseItem->appendRow(subjectItem);

            // [3] 创建学科下的版本
            QJsonArray versions = json.getJsonArray("versions", subject);
            for (QJsonArray::const_iterator viter = versions.begin(); viter != versions.end(); ++viter) {
                QJsonObject version = viter->toObject();
                QString versionName = json.getString("title", "", version);
                QStandardItem *versionItem = Service::createVersionItem(versionName);
                subjectItem->appendRow(versionItem);

                // 4. 创建版本下的教材
                QJsonArray books = json.getJsonArray("books", version);
                for (QJsonArray::const_iterator biter = books.begin(); biter != books.end(); ++biter) {
                    QJsonObject book  = biter->toObject();
                    QString bookName  = json.getString("title", "", book);
                    QString bookCode  = json.getString("code", "",  book);
                    QString bookCover = json.getString("cover", "", book);
                    versionItem->appendRow(Service::createBookItem(bookName, bookCode, bookCover));
                }
            }
        }
    }
}

// 读取教材的章节目录
void BookService::readBookChapters(const QString &bookCode) {
    QFileInfo chapterFileInfo(Service::bookChaptersFilePath(booksDir, bookCode)); // 教材文件信息

    if (!chapterFileInfo.exists()) {
        return;
    }

    chaptersModel->removeRows(0, chaptersModel->rowCount());
    Json json(chapterFileInfo.absoluteFilePath(), true);
    QJsonArray chapters = json.getJsonArray("chapters");

    for (QJsonArray::const_iterator iter = chapters.begin(); iter != chapters.end(); ++iter) {
        QJsonObject chapter = iter->toObject();
        createChapterItems(json, chapter, NULL);
    }
}

/**
 * 创建教程的章节目录
 *
 * @param json 教材的 JSON 对象
 * @param currentChapter 当前章节的对象
 * @param parentChapterNameItem 父章节的 item，使用 currentChapter 创建它的子 item
 */
void BookService::createChapterItems(const Json &json, const QJsonObject &currentChapter, QStandardItem *parentChapterNameItem) {
    // 注：每个章节目录都包含 2 列：名字和编码
    bool kp = json.getBool("kp", false, currentChapter); // true 表示是知识点，false 表示章节
    QString name   = json.getString("title", "",  currentChapter);
    QString code   = json.getString("code", "",   currentChapter);
    QString kpCode = json.getString("kpCode", "", currentChapter);
    QString kpSubjectCode = json.getString("kpSubjectCode", "", currentChapter);

    QList<QStandardItem*> items;

    if (kp) {
        items = Service::createKpItemsForChapter(name, kpCode, kpSubjectCode);
    } else {
        items = { new QStandardItem(name), new QStandardItem(code) };
    }

    if (NULL == parentChapterNameItem) {
        // parentItem 为 NULL，表示需要创建树的根节点
        chaptersModel->appendRow(items);
        parentChapterNameItem = items.at(0);
    } else {
        // parentItem 不为 NULL，表示创建它的子节点
        parentChapterNameItem->appendRow(items);
        parentChapterNameItem = items.at(0);
    }

    // 章节下的章节
    QJsonArray subchapters = json.getJsonArray("children", currentChapter);
    for (QJsonArray::const_iterator iter = subchapters.begin(); iter != subchapters.end(); ++iter) {
        QJsonObject subchapter = iter->toObject();
        createChapterItems(json, subchapter, parentChapterNameItem);
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
    // 1. 遍历教材信息的树，对每一个教材生成一个 CodeInfo 对象，info 为: 阶段 > 学科 > 版本 > 教材，生成 booksInfo 列表
    // 2. 校验 booksInfo 中的 code，看是否被使用过
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

                    // 编码为非空的才参与校验
                    if (!bookCode.isEmpty()) {
                        booksInfo.append(CodeInfo(bookCode, bookInfo));
                    }
                }
            }
        }
    }

    return CodeInfoService::validateCodeInfos(&booksInfo, error);
}

/**
 * 如果当前教材的章节编码都是唯一的，则验证通过返回 true，如果编码被重复使用验证不通过返回 false
 *
 * @param error 校验不通过的错误信息，不能为 NULL
 * @return 校验通过返回 true，不通过返回 false
 */
bool BookService::validateChapters(QString *error) const {
    QList<CodeInfo> chaptersInfo = CodeInfoService::getCodeInfos(chaptersModel);
    return CodeInfoService::validateCodeInfos(&chaptersInfo, error);
}

// 当 previous 为 true 时在 current 节点前插入章节，否则在 current 后插入章节
void BookService::insertChapter(const QModelIndex &current, bool previous) {
    // 如果 parent 有效，则插入到 parent 下面
    // 如果 parent 无效，则说明 current 是第一级节点，插入到 chaptersModel 下面

    QModelIndex parent = current.parent();
    QString code = Service::generateHierarchicalCode(chaptersModel, parent, 1);
    QList<QStandardItem*> childColumns = { new QStandardItem("新建章节"), new QStandardItem(code) };
    int row = previous ? current.row() : current.row() + 1; // 插入的位置

    if (parent.isValid()) {
        // 在节点前|后插入章节
        chaptersModel->itemFromIndex(parent)->insertRow(row, childColumns);
    } else {
        // 插入第一级的章节
        chaptersModel->insertRow(row, childColumns);
    }
}

// 在 parent 节点下增加子章节
void BookService::appendChildChapter(const QModelIndex &parent) {
    QString code = Service::generateHierarchicalCode(chaptersModel, parent, 1);
    QList<QStandardItem*> childColumns = { new QStandardItem("新建章节"), new QStandardItem(code) };

    if (parent.isValid()) {
        chaptersModel->itemFromIndex(parent)->appendRow(childColumns);
    } else {
        chaptersModel->appendRow(childColumns);
    }
}

// 在 parent 下增加所属的知识点节点
void BookService::appendKpOfChapter(const QModelIndex &parent, const QString &kpName, const QString &kpCode, const QString &kpSubjectCode) {
    if (!parent.isValid()) { return; }

    chaptersModel->itemFromIndex(parent)->appendRow(Service::createKpItemsForChapter(kpName, kpCode, kpSubjectCode));
}

// 保存教材章节内容
bool BookService::saveBookChapters(const QString &bookCode,
                                   const QString &bookSubject,
                                   const QString &bookVersion,
                                   const QString &bookName,
                                   const QString &bookCover) {
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
    book.insert("title",    bookName);
    book.insert("cover",    bookCover);
    book.insert("chapters", chaptersJson);

    // 保存到文件
    return Util::writeStringToFile(QJsonDocument(book).toJson(), Service::bookChaptersFilePath(booksDir, bookCode));
}

bool BookService::saveBooks() {
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
    return Util::writeStringToFile(QJsonDocument(root).toJson(), Service::booksFilePath(booksDir));
}

// 创建章节目录对应的 JSON 对象
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

    if (childrenChapters.size() > 0) {
        chapterJson.insert("children", childrenChapters);
    }

    // 如果是知识点则保持它的信息
    QModelIndex kpIndex = chaptersModel->indexFromItem(chapterNameItem);
    if (Service::isKpIndex(kpIndex)) {
        QString kpCode = kpIndex.data(ROLE_CODE).toString();
        QString kpName = chapterName;
        QString kpSubjectCode = kpIndex.data(ROLE_CODE_EXT).toString();
        chapterJson.insert("kp",     true);
        chapterJson.insert("kpCode", kpCode);
        chapterJson.insert("kpName", kpName);
        chapterJson.insert("kpSubjectCode", kpSubjectCode);
    }

    return chapterJson;
}

