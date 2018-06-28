#include "KpService.h"
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

// 判断 index 是否阶段对应的 index
bool KpService::isPhaseIndex(const QModelIndex &index) const {
    if (!index.isValid()) {
        return false;
    }

    return TYPE_PHASE == index.data(ROLE_TYPE).toString();
}

// 判断 index 是否学科对应的 index
bool KpService::isSubjectIndex(const QModelIndex &index) const {
    if (!index.isValid()) {
        return false;
    }

    return TYPE_SUBJECT == index.data(ROLE_TYPE).toString();
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
        QStandardItem *phaseItem = new QStandardItem(phaseName);
        phaseItem->setData(TYPE_PHASE, ROLE_TYPE); // 表示教学阶段
        subjectsModel->appendRow(phaseItem);

        // [2] 创建教学阶段下的学科
        QJsonArray subjects = json.getJsonArray("subjects", phase);
        for (QJsonArray::const_iterator siter = subjects.begin(); siter != subjects.end(); ++siter) {
            // 创建学科
            QJsonObject subject = siter->toObject();
            QString subjectName = json.getString("title", "", subject);
            QString subjectCode = json.getString("code", "", subject);
            QStandardItem *subjectItem = new QStandardItem(subjectName);
            subjectItem->setData(TYPE_SUBJECT, ROLE_TYPE); // 表示学科
            subjectItem->setData(subjectCode, ROLE_CODE);  // 学科编码
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
    subjectKps.insert("subject", subjectName);
    subjectKps.insert("code",    subjectCode);
    subjectKps.insert("kps",     kpsJson);

    // 保存到文件
    return Util::writeStringToFile(QJsonDocument(subjectKps).toJson(), kpsDir.filePath(subjectCode + ".json"));
}

// 读取知识点到知识点树中
void KpService::readSubjectKps(const QString &path) {
    QFile file(path);
    if (!file.exists()) {
        return;
    }

    kpsModel->removeRows(0, kpsModel->rowCount());
    Json json(path, true);
    QJsonArray kps = json.getJsonArray("kps");

    for (QJsonArray::const_iterator iter = kps.begin(); iter != kps.end(); ++iter) {
        QJsonObject kp = iter->toObject();
        createKps(json, kp, NULL);
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

// 创建学科知识点的 JSON
QJsonObject KpService::createSubjectKpsJson(QStandardItem *kpNameItem, QStandardItem *kpCodeItem) {
    if (NULL == kpNameItem || NULL == kpCodeItem) { return QJsonObject(); }

    QString kpName = kpNameItem->data(Qt::DisplayRole).toString();
    QString kpCode = kpCodeItem->data(Qt::DisplayRole).toString();

    // 递归遍历子章节
    QJsonArray childrenKps;
    for (int i = 0; i < kpNameItem->rowCount(); ++i) {
        QStandardItem *childKpNameItem = kpNameItem->child(i, 0);
        QStandardItem *childKpCodeItem = kpNameItem->child(i, 1);
        childrenKps.append(createSubjectKpsJson(childKpNameItem, childKpCodeItem));
    }

    QJsonObject kpJson;
    kpJson.insert("title", kpName);
    kpJson.insert("code",  kpCode);
    kpJson.insert("children", childrenKps);

    return kpJson;
}

// 创建知识点树
void KpService::createKps(const Json &json, const QJsonObject &currentKp, QStandardItem *parentKpNameItem) {
    // 注：每个知识点目录都包含 2 列：名字和编码
    QString name = json.getString("title", "", currentKp);
    QString code = json.getString("code", "",  currentKp);
    QList<QStandardItem*> childColumns;

    if (NULL == parentKpNameItem) {
        // parentItem 为 NULL，表示需要创建树的根节点
        parentKpNameItem = new QStandardItem(name);
        childColumns << parentKpNameItem << new QStandardItem(code);
        kpsModel->appendRow(childColumns);
    } else {
        // parentItem 不为 NULL，表示创建它的子节点
        QStandardItem *currentKpNameItem = new QStandardItem(name);
        childColumns << currentKpNameItem << new QStandardItem(code);
        parentKpNameItem->appendRow(childColumns);
        parentKpNameItem = currentKpNameItem;
    }

    // 子知识点
    QJsonArray subKps = json.getJsonArray("children", currentKp);
    for (QJsonArray::const_iterator iter = subKps.begin(); iter != subKps.end(); ++iter) {
        QJsonObject subKp = iter->toObject();
        createKps(json, subKp, parentKpNameItem);
    }
}
