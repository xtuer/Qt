#include "KpService.h"
#include "Service.h"
#include "CodeInfoService.h"
#include "bean/Book.h"
#include "util/Json.h"
#include "util/Util.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QStandardItemModel>

KpService::KpService(QStandardItemModel *subjectsModel, QStandardItemModel *kpsModel)
    : subjectsModel(subjectsModel), kpsModel(kpsModel) {
}

// 保存学科结构
bool KpService::saveSubjects(const QDir &kpsDir) {
    QJsonArray phases;

    // 逐层遍历访问得到教材信息，保存到数组 booksInfo 中
    for (int i = 0; i < subjectsModel->rowCount(); ++i) {
        // 阶段
        QStandardItem *phaseItem = subjectsModel->item(i, 0);
        QString phaseName = phaseItem->data(Qt::DisplayRole).toString();

        QJsonArray subjects;
        for (int j = 0; j < phaseItem->rowCount(); ++j) {
            // 学科
            QStandardItem *subjectItem = phaseItem->child(j, 0);
            QString subjectName = subjectItem->data(Qt::DisplayRole).toString();
            QString subjectCode = subjectItem->data(ROLE_CODE).toString();

            QJsonObject subject;
            subject.insert("title", subjectName);
            subject.insert("code", subjectCode);
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
    return Util::writeStringToFile(QJsonDocument(root).toJson(), kpsDir.filePath("kps.json"));
}

// 读取学科结构到学科树中
void KpService::readSubjects(const QString &path) {
    // 1. 创建教学阶段
    // 2. 创建每个教学阶段下的学科
    // *  每个 item 都设定一个 ROLE_TYPE 的类型数据，用于判断显示对应的右键菜单

    subjectsModel->removeRows(0, subjectsModel->rowCount());
    Json json(path, true);

    QJsonArray phases = json.getJsonArray("phases");
    for (QJsonArray::const_iterator iter = phases.begin(); iter != phases.end(); ++iter) {
        // [1] 创建教学阶段的节点
        QJsonObject phase = iter->toObject();
        QString phaseName = json.getString("title", "", phase);
        QStandardItem *phaseItem = Service::createPhaseItem(phaseName);
        subjectsModel->appendRow(phaseItem);

        // [2] 创建教学阶段下的学科
        QJsonArray subjects = json.getJsonArray("subjects", phase);
        for (QJsonArray::const_iterator siter = subjects.begin(); siter != subjects.end(); ++siter) {
            // 创建学科
            QJsonObject subject = siter->toObject();
            QString subjectName = json.getString("title", "", subject);
            QString subjectCode = json.getString("code", "", subject);
            QStandardItem *subjectItem = Service::createSubjectItem(subjectName, subjectCode);
            phaseItem->appendRow(subjectItem);
        }
    }
}

// 保存学科的知识点
bool KpService::saveSubjectKps(const QString &subjectName, const QString &subjectCode, const QDir &kpsDir) {
    QJsonArray kpsJson;
    for (int i = 0; i < kpsModel->rowCount(); ++i) {
        QStandardItem *kpNameItem = kpsModel->item(i, 0);
        QStandardItem *kpCodeItem = kpsModel->item(i, 1);

        kpsJson.append(createSubjectKpsJson(kpNameItem, kpCodeItem));
    }

    QJsonObject subjectKps;
    subjectKps.insert("subject",     subjectName);
    subjectKps.insert("subjectCode", subjectCode);
    subjectKps.insert("kps",  kpsJson);

    // 保存到文件
    return Util::writeStringToFile(QJsonDocument(subjectKps).toJson(), Service::kpFilePath(kpsDir, subjectCode));
}

// 读取知识点到知识点树中
void KpService::readSubjectKps(const QString &path, bool withChapter) {
    QFile file(path);
    if (!file.exists()) {
        return;
    }

    kpsModel->removeRows(0, kpsModel->rowCount());
    Json json(path, true);
    QJsonArray kps = json.getJsonArray("kps");

    for (QJsonArray::const_iterator iter = kps.begin(); iter != kps.end(); ++iter) {
        QJsonObject kp = iter->toObject();
        createKpItems(json, kp, NULL, withChapter);
    }
}

// 如果学科的编码都是唯一的，则验证通过返回 true，如果编码被重复使用验证不通过返回 false
bool KpService::validateSubjects(const QModelIndex &currentIndex, const QString &editingSubjectCode, QString *error) const {
    // 1. 遍历学科结构的树，对每一个学科生成一个 CodeInfo 对象，info 为: 阶段 > 学科，生成 subjectsInfo 列表
    // 2. 校验 subjectsInfo 中的 code，看是否被使用过
    QList<CodeInfo> subjectsInfo;

    // 逐层遍历访问得到学科信息，保存到数组 subjectsInfo 中
    for (int i = 0; i < subjectsModel->rowCount(); ++i) {
        // 阶段
        QStandardItem *phaseItem = subjectsModel->item(i, 0);
        QString phaseName = phaseItem->data(Qt::DisplayRole).toString();

        if (phaseName.trimmed().isEmpty()) {
            error->append("阶段不能为空\n");
            return false;
        }

        for (int j = 0; j < phaseItem->rowCount(); ++j) {
            // 学科
            QStandardItem *subjectItem = phaseItem->child(j, 0);
            QString subjectCode = subjectItem->data(ROLE_CODE).toString();
            QString subjectName = subjectItem->data(Qt::DisplayRole).toString();
            QString subjectInfo = QString("%1 > %2").arg(phaseName).arg(subjectName);

            if (subjectName.trimmed().isEmpty()) {
                error->append("学科的名字不能为空\n");
                return false;
            }

            // 选中的学科的 code 更新为编辑的教材 code，这样就能够判断当前编辑的 code 是否可用
            if (currentIndex.isValid() && subjectsModel->itemFromIndex(currentIndex) == subjectItem) {
                subjectCode = editingSubjectCode.trimmed();
            }

            // 编码为非空的才参与校验
            if (!subjectCode.isEmpty()) {
                subjectsInfo.append(CodeInfo(subjectCode, subjectInfo));
            }
        }
    }

    return CodeInfoService::validateCodeInfos(&subjectsInfo, error);
}

// 如果当前学科的知识点的编码都是唯一的，则验证通过返回 true，如果编码被重复使用验证不通过返回 false
bool KpService::validateSubjectKps(QString *error) const {
    QList<CodeInfo> kpsInfo = CodeInfoService::getCodeInfos(kpsModel);
    return CodeInfoService::validateCodeInfos(&kpsInfo, error);
}

// 在 parent 节点下增加子知识点
void KpService::appendChildKp(const QModelIndex &parent) {
    QString code = Service::generateHierarchicalCode(kpsModel, parent, 1);
    QList<QStandardItem*> childColumns = Service::createKpItems("新建知识点", code);

    if (parent.isValid()) {
        kpsModel->itemFromIndex(parent)->appendRow(childColumns);
    } else {
        kpsModel->appendRow(childColumns);
    }
}

void KpService::insertKp(const QModelIndex &current, bool previous) {
    // 如果 parent 有效，则插入到 parent 下面
    // 如果 parent 无效，则说明 current 是第一级节点，插入到 kpsModel 下面

    QModelIndex parent = current.parent();
    QString code = Service::generateHierarchicalCode(kpsModel, parent, 1);
    QList<QStandardItem*> childColumns = Service::createKpItems("新建知识点", code);
    int row = previous ? current.row() : current.row() + 1; // 插入的位置

    if (parent.isValid()) {
        // 在节点前|后插入知识点
        kpsModel->itemFromIndex(parent)->insertRow(row, childColumns);
    } else {
        // 插入第一级的知识点
        kpsModel->insertRow(row, childColumns);
    }
}

// 创建学科知识点的 JSON
QJsonObject KpService::createSubjectKpsJson(QStandardItem *nameItem, QStandardItem *codeItem) {
    if (NULL == nameItem || NULL == codeItem) { return QJsonObject(); }

    QModelIndex nameIndex = kpsModel->indexFromItem(nameItem);

    QString name = nameItem->data(Qt::DisplayRole).toString();
    QString code = codeItem->data(Qt::DisplayRole).toString();
    QString cognitionMust = nameIndex.sibling(nameItem->row(), 2).data().toString();
    QString cognitionOptional = nameIndex.sibling(nameItem->row(), 3).data().toString();
    QString qualityStudy = nameIndex.sibling(nameItem->row(), 4).data().toString();
    QString qualityLevel = nameIndex.sibling(nameItem->row(), 5).data().toString();

    // 递归遍历子章节
    QJsonArray childrenKps;
    for (int i = 0; i < nameItem->rowCount(); ++i) {
        QStandardItem *childNameItem = nameItem->child(i, 0);
        QStandardItem *childCodeItem = nameItem->child(i, 1);
        childrenKps.append(createSubjectKpsJson(childNameItem, childCodeItem));
    }

    // 所有节点都有 title 和 code，但是不一定有 认知发展(必修)、认知发展(选修)、学业质量参考(学业考)、学业质量参考(等级考)
    QJsonObject kpJson;
    kpJson.insert("title", name);
    kpJson.insert("code",  code);

    if (!cognitionMust.isEmpty()) {
        kpJson.insert("cognitionMust", cognitionMust); // 认知发展(必修)
    }
    if (!cognitionOptional.isEmpty()) {
        kpJson.insert("cognitionOptional", cognitionOptional); // 认知发展(选修)
    }
    if (!qualityStudy.isEmpty()) {
        kpJson.insert("qualityStudy", qualityStudy); // 学业质量参考(学业考)
    }
    if (!qualityLevel.isEmpty()) {
        kpJson.insert("qualityLevel", qualityLevel); // 学业质量参考(等级考)
    }
    if (childrenKps.size() > 0) {
        kpJson.insert("children", childrenKps);
    }

    // 如果是章节则保持对应的信息
    QModelIndex chapterIndex = kpsModel->indexFromItem(nameItem);
    if (Service::isChapterIndex(chapterIndex)) {
        QString chapterName = name;
        QString chapterCode = chapterIndex.data(ROLE_CODE).toString();
        QString chapterBookCode = chapterIndex.data(ROLE_CODE_EXT).toString();

        kpJson.insert("chapter", true);
        kpJson.insert("chapterName", chapterName);
        kpJson.insert("chapterCode", chapterCode);
        kpJson.insert("chapterBookCode", chapterBookCode);
    }

    return kpJson;
}

// 创建知识点树
void KpService::createKpItems(const Json &json, const QJsonObject &currentKp, QStandardItem *parentKpNameItem, bool withChapter) {
    // 注：每个知识点目录都包含 2 列：名字和编码
    QString name = json.getString("title", "", currentKp);
    QString code = json.getString("code", "",  currentKp);
    QString cognitionMust     = json.getString("cognitionMust", "",     currentKp); // 认知发展(必修)
    QString cognitionOptional = json.getString("cognitionOptional", "", currentKp); // 认知发展(选修)
    QString qualityStudy      = json.getString("qualityStudy", "",      currentKp); // 学业质量参考(学业考)z
    QString qualityLevel      = json.getString("qualityLevel", "",      currentKp); // 学业质量参考(等级考)
    bool chapter = json.getBool("chapter", false, currentKp); // true 表示是章节，false 表示知识点
    QList<QStandardItem*> items;

    if (chapter) {
        if (!withChapter) { return; }

        QString chapterName     = json.getString("chapterName", "",     currentKp);
        QString chapterCode     = json.getString("chapterCode", "",     currentKp);
        QString chapterBookCode = json.getString("chapterBookCode", "", currentKp);
        items = Service::createChapterItemsForKp(chapterName, chapterCode, chapterBookCode);
    } else {
        items = Service::createKpItems(name, code, cognitionMust, cognitionOptional, qualityStudy, qualityLevel);
    }

    if (NULL == parentKpNameItem) {
        // parentItem 为 NULL，表示需要创建树的根节点
        kpsModel->appendRow(items);
        parentKpNameItem = items.at(0);
    } else {
        // parentItem 不为 NULL，表示创建它的子节点
        parentKpNameItem->appendRow(items);
        parentKpNameItem = items.at(0);
    }

    // 子知识点
    QJsonArray subKps = json.getJsonArray("children", currentKp);
    for (QJsonArray::const_iterator iter = subKps.begin(); iter != subKps.end(); ++iter) {
        QJsonObject subKp = iter->toObject();
        createKpItems(json, subKp, parentKpNameItem, withChapter);
    }
}
