#include "Service.h"
#include "bean/Book.h"
#include <QDebug>

// 给 parent 下的新节点生成有层级关系的编码
// 新节点的编码为: parentCode + (子节点最大编码的后 2 位+1)，例如:
// 01
//     0101
//     0102
//         010201
//         010202
// 02
//     0201
//     0202
QString Service::generateHierarchicalCode(QStandardItemModel *model, const QModelIndex &parent, int codeColumn) {
    // 1. 遍历 parent 所有的子节点
    // 2. 截取子节点的 code 最后 2 位转换为整数，求最大的整数 max
    // 3. max=max+1 为新节点的 code 最后 2 位
    // 4. 新节点的 code 为 parent 的 code 加上 max (转换为 2 位的字符串，不足则前面补 0)

    int max = 0;
    int rowCount = model->rowCount(parent);

    // [1] 遍历 parent 所有的子节点
    // [2] 截取子节点的 code 最后 2 位转换为整数，求最大的整数 max
    for (int row = 0; row < rowCount; ++row) {
        QString code = model->index(row, codeColumn, parent).data().toString();
        int temp = code.right(2).toInt();
        max = temp > max ? temp : max;
    }

    // [3] max=max+1 为新节点的 code 最后 2 位
    // [4] 新节点的 code 为 parent 的 code 加上 max (转换为 2 位的字符串，不足则前面补 0)
    max += 1;
    QString parentCode = parent.sibling(parent.row(), codeColumn).data().toString();
    QString childCode  = QString("%1%2").arg(parentCode).arg(max, 2, 10, QChar('0'));

    return childCode;
}

// 判断 index 是否教材对应的 index
bool Service::isBookIndex(const QModelIndex &index) {
    return isIndexOfType(index, TYPE_BOOK);
}

// 判断 index 是否阶段对应的 index
bool Service::isPhaseIndex(const QModelIndex &index) {
    return isIndexOfType(index, TYPE_PHASE);
}

// 判断 index 是否学科对应的 index
bool Service::isSubjectIndex(const QModelIndex &index) {
    return isIndexOfType(index, TYPE_SUBJECT);
}

// 判断 index 是否版本对应的 index
bool Service::isVersionIndex(const QModelIndex &index) {
    return isIndexOfType(index, TYPE_VERSION);
}

// 判断 index 是否知识点对应的 index
bool Service::isKpIndex(const QModelIndex &index) {
    return isIndexOfType(index, TYPE_KP);
}

// 判断 index 是否章节对应的 index
bool Service::isChapterIndex(const QModelIndex &index) {
    return isIndexOfType(index, TYPE_CHAPTER);
}

// 判断 index 是否 type 指定的类型
bool Service::isIndexOfType(const QModelIndex &index, const QString &type) {
    if (!index.isValid()) {
        return false;
    }

    return type == index.data(ROLE_TYPE).toString();
}

// 创建阶段的 item
QStandardItem* Service::createPhaseItem(const QString &name) {
    QStandardItem *phaseItem = new QStandardItem(name);
    phaseItem->setData(TYPE_PHASE, ROLE_TYPE); // 表示阶段节点
    // phaseItem->setIcon(QIcon("image/common/phase.png"));

    return phaseItem;
}

// 创建学科的 item
QStandardItem* Service::createSubjectItem(const QString &name, const QString &code) {
    QStandardItem *subjectItem = new QStandardItem(name);
    subjectItem->setData(TYPE_SUBJECT, ROLE_TYPE); // 表示学科节点
    subjectItem->setData(code, ROLE_CODE);
    // subjectItem->setIcon(QIcon("image/common/subject.png"));

    return subjectItem;
}

// 创建版本的 item
QStandardItem* Service::createVersionItem(const QString &name) {
    QStandardItem *versionItem = new QStandardItem(name);
    versionItem->setData(TYPE_VERSION, ROLE_TYPE); // 表示版本节点
    // versionItem->setIcon(QIcon("image/common/version.png"));

    return versionItem;
}

// 创建教材的 item
QStandardItem* Service::createBookItem(const QString &name, const QString &code, const QString &cover) {
    QStandardItem *bookItem = new QStandardItem(name);
    bookItem->setData(TYPE_BOOK, ROLE_TYPE); // 表示教材节点
    bookItem->setData(code,      ROLE_CODE);
    bookItem->setData(cover,     ROLE_COVER);
    bookItem->setIcon(QIcon("image/common/book.png"));

    return bookItem;
}

// 创建章节目录中使用的知识点 item
QList<QStandardItem *> Service::createKpItemsForChapter(const QString &name, const QString &code, const QString &subjectCode) {
    // 知识点的 item: 有图标、不可编辑
    QStandardItem *nameItem = new QStandardItem(name);
    nameItem->setData(TYPE_KP,     ROLE_TYPE);
    nameItem->setData(code,        ROLE_CODE);
    nameItem->setData(subjectCode, ROLE_CODE_EXT);
    nameItem->setIcon(QIcon("image/common/kp.png"));
    nameItem->setEditable(false);

    QStandardItem *codeItem = new QStandardItem();
    codeItem->setEditable(false);

    return { nameItem, codeItem };
}

// 创建知识点树中使用的章节目录 item
QList<QStandardItem *> Service::createChapterItemsForKp(const QString &name, const QString &code, const QString &bookCode) {
    // 章节目录的 item: 有图标、不可编辑
    QStandardItem *nameItem = new QStandardItem(name);
    nameItem->setData(TYPE_CHAPTER, ROLE_TYPE);
    nameItem->setData(code,         ROLE_CODE);
    nameItem->setData(bookCode,     ROLE_CODE_EXT);
    nameItem->setIcon(QIcon("image/common/chapter.png"));
    nameItem->setToolTip(QString("教材编码: %1，章节: %2").arg(bookCode).arg(name));

    QStandardItem *codeItem = new QStandardItem();
    QStandardItem *cognitionMustItem = new QStandardItem();
    QStandardItem *cognitionOptionalItem = new QStandardItem();
    QStandardItem *qualityStudyItem = new QStandardItem();
    QStandardItem *qualityLevelItem = new QStandardItem();

    nameItem->setEditable(false);
    codeItem->setEditable(false);
    cognitionMustItem->setEditable(false);
    cognitionOptionalItem->setEditable(false);
    qualityStudyItem->setEditable(false);
    qualityLevelItem->setEditable(false);

    return { nameItem, codeItem, cognitionMustItem, cognitionOptionalItem, qualityStudyItem, qualityLevelItem };
}

// 创建知识点表中一个知识点对应一行的多个 items
QList<QStandardItem *> Service::createKpItems(const QString &name, const QString &code,
                                              const QString &cognitionMust, const QString &cognitionOptional,
                                              const QString &qualityStudy, const QString &qualityLevel) {
    QStandardItem *nameItem = new QStandardItem(name);
    QStandardItem *codeItem = new QStandardItem(code);

    QStandardItem *cognitionMustItem = new QStandardItem(cognitionMust);
    QStandardItem *cognitionOptionalItem = new QStandardItem(cognitionOptional);
    QStandardItem *qualityStudyItem = new QStandardItem(qualityStudy);
    QStandardItem *qualityLevelItem = new QStandardItem(qualityLevel);

    return { nameItem, codeItem, cognitionMustItem, cognitionOptionalItem, qualityStudyItem, qualityLevelItem };
}

// 向 parent 下添加子节点 items
void Service::appendRow(QStandardItemModel *model, const QModelIndex &parent, const QList<QStandardItem*> &items) {
    if (parent.isValid()) {
        model->itemFromIndex(parent)->appendRow(items);
    } else {
        model->appendRow(items);
    }
}

// 学段学科结构的文件路径
QString Service::booksFilePath(const QDir &bookDir) {
    return bookDir.filePath("books.json");
}

// 教材章节的文件路径
QString Service::bookChaptersFilePath(const QDir &bookDir, const QString &bookCode) {
    return bookDir.filePath(bookCode + ".json");
}

// 学段学科结构的文件路径
QString Service::subjectsFilePath(const QDir &kpDir) {
    return kpDir.filePath("kps.json");
}

// 学科知识点的文件路径
QString Service::subjectKpsFilePath(const QDir &kpDir, const QString &subjectCode) {
    return kpDir.filePath(subjectCode + ".json");
}
