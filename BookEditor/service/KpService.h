#ifndef KPSERVICE_H
#define KPSERVICE_H

#include <QDir>
#include <QModelIndex>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QJsonObject>

class Json;

class KpService {
public:
    KpService(QStandardItemModel *subjectsModel, QStandardItemModel *kpsModel);

    bool isPhaseIndex(const QModelIndex &index) const;   // 判断 index 是否阶段对应的 index
    bool isSubjectIndex(const QModelIndex &index) const; // 判断 index 是否阶段对应的 index

    bool saveSubjects(const QDir &kpsDir);  // 保存学科结构
    void readSubjects(const QString &path); // 读取学科结构到学科树中

    bool saveSubjectKps(const QString &subjectName, const QString &subjectCode, const QDir &kpsDir); // 保存学科的知识点
    void readSubjectKps(const QString &path); // 读取知识点到知识点树中

    /**
     * 如果学科的编码都是唯一的，则验证通过返回 true，如果编码被重复使用验证不通过返回 false
     *
     * @param currentIndex 学科树中当前被选中节点的 index
     * @param editingSubjectCode 正在编辑的学科编码
     * @param error 校验不通过的错误信息，不能为 NULL
     * @return 校验通过返回 true，不通过返回 false
     */
    bool validateSubjects(const QModelIndex &currentIndex, const QString &editingSubjectCode, QString *error) const;

    /**
     * 如果当前学科的知识点的编码都是唯一的，则验证通过返回 true，如果编码被重复使用验证不通过返回 false
     *
     * @param error 校验不通过的错误信息，不能为 NULL
     * @return 校验通过返回 true，不通过返回 false
     */
    bool validateSubjectKps(QString *error) const;

private:
    QJsonObject createSubjectKpsJson(QStandardItem *kpNameItem, QStandardItem *kpCodeItem); // 创建学科知识点的 JSON

    /**
     * 创建知识点树
     *
     * @param json 知识点的 JSON 对象
     * @param currentKp 当前知识点
     * @param parentKpNameItem 父知识点的 item，使用 currentKp 创建它的子 item
     */
    void createKps(const Json &json, const QJsonObject &currentKp, QStandardItem *parentKpNameItem);

    QStandardItemModel *subjectsModel = 0; // 学科的 model
    QStandardItemModel *kpsModel      = 0; // 知识点 model
};

#endif // KPSERVICE_H
