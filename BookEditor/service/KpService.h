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
    KpService(QStandardItemModel *subjectsModel, QStandardItemModel *kpsModel, const QDir &kpsDir);

    bool saveSubjects();  // 保存学科结构
    bool saveSubjectKps(const QString &subjectName, const QString &subjectCode); // 保存学科的知识点
    void readSubjects(); // 读取学科结构到学科树中
    void readSubjectKps(const QString &subjectCode, bool withChapter); // 读取知识点到知识点树中

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

    /**
     * 在 parent 节点下增加子知识点
     *
     * @param parent
     */
    void appendChildKp(const QModelIndex &parent, const QString &subjectCode);

    /**
     * 当 previous 为 true 时在 current 节点前插入知识点，否则在 current 后插入知识点
     *
     * @param current
     * @param previous
     */
    void insertKp(const QModelIndex &current, bool previous, const QString &subjectCode);

private:
    QJsonObject createSubjectKpsJson(QStandardItem *nameItem, QStandardItem *codeItem); // 创建学科知识点的 JSON

    /**
     * 生成知识点的编码
     *
     * @param subjectCode 学科编码
     * @param parent 父知识点
     * @return 返回知识点的编码
     */
    QString generateKpCode(const QString &subjectCode, const QModelIndex &parent);

    /**
     * 创建知识点树
     *
     * @param json 知识点的 JSON 对象
     * @param currentKp 当前知识点
     * @param parentKpNameItem 父知识点的 item，使用 currentKp 创建它的子 item
     * @param withChapter 为 true 时才显示知识点的章节
     */
    void createKpItems(const Json &json, const QJsonObject &currentKp, QStandardItem *parentKpNameItem, bool withChapter);

    QDir kpsDir; // 学科知识点保存的文件夹
    QStandardItemModel *subjectsModel = nullptr; // 学科的 model
    QStandardItemModel *kpsModel      = nullptr; // 知识点 model
};

#endif // KPSERVICE_H
