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
    int max = 0;
    int rowCount = model->rowCount(parent);

    // 查找子节点最大编码的后 2 位
    for (int row = 0; row < rowCount; ++row) {
        QString code = model->index(row, codeColumn, parent).data().toString();
        int temp = code.right(2).toInt();
        max = temp > max ? temp : max;
    }

    max += 1;

    if (parent.isValid()) {
        // 非第一级节点的编码
        QString parentCode = parent.sibling(parent.row(), codeColumn).data().toString();
        QString childCode  = QString("%1%2").arg(parentCode).arg(max, 2, 10, QChar('0'));

        return childCode;
    } else {
        // 第一级节点的编码
        return QString("%1").arg(max, 2, 10, QChar('0'));
    }
}

// 判断 index 是否教材对应的 index
bool Service::isBookIndex(const QModelIndex &index) {
    if (!index.isValid()) {
        return false;
    }

    return TYPE_BOOK == index.data(ROLE_TYPE).toString();
}

// 判断 index 是否阶段对应的 index
bool Service::isPhaseIndex(const QModelIndex &index) {
    if (!index.isValid()) {
        return false;
    }

    return TYPE_PHASE == index.data(ROLE_TYPE).toString();
}

// 判断 index 是否学科对应的 index
bool Service::isSubjectIndex(const QModelIndex &index) {
    if (!index.isValid()) {
        return false;
    }

    return TYPE_SUBJECT == index.data(ROLE_TYPE).toString();
}

// 判断 index 是否版本对应的 index
bool Service::isVersionIndex(const QModelIndex &index) {
    if (!index.isValid()) {
        return false;
    }

    return TYPE_VERSION == index.data(ROLE_TYPE).toString();
}

// 判断 index 是否知识点对应的 index
bool Service::isKpIndex(const QModelIndex &index) {
    if (!index.isValid()) {
        return false;
    }

    return TYPE_KP == index.data(ROLE_TYPE).toString();
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
    bookItem->setIcon(QIcon("image/common/book2.png"));

    return bookItem;
}

// 创建章节目录中使用的知识点 item
QStandardItem* Service::createKpItem(const QString &name, const QString &code) {
    // 知识点的 item: 有图标、不可编辑
    QStandardItem *item = new QStandardItem(name);
    item->setData(TYPE_KP, ROLE_TYPE);
    item->setData(code,    ROLE_CODE);
    item->setIcon(QIcon("image/common/kp.png"));
    item->setEditable(false);

    return item;
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
