#ifndef KPEDITOR_H
#define KPEDITOR_H

#include <QWidget>
#include <QModelIndex>
#include <QDir>

class KpService;
class QStandardItemModel;

namespace Ui {
class KpEditor;
}

/**
 * 知识点编辑
 */
class KpEditor : public QWidget {
    Q_OBJECT

public:
    explicit KpEditor(QWidget *parent = 0);
    ~KpEditor();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    void initialize();   // 初始化
    void handleEvents(); // 事件处理
    void createSubjectsContextMenu(); // 创建左侧学科的右键菜单
    void createKpsContextMenu();      // 创建中间知识点右键菜单
    void resetKps(); // 重置知识点

    void save(); // 调用 saveSubjects() and saveKps()
    void openSubjects(); // 打开学科到左侧的教材目录树中
    void openSubjectKps(const QString &subjectCode); // 打开学科的知识点到右侧的知识点树中

    bool validate() const; // 校验学科和知识点的编码是否唯一，未被重复使用

    Ui::KpEditor *ui;
    QStandardItemModel *subjectsModel = NULL; // 学科的 model
    QStandardItemModel *kpsModel      = NULL; // 知识点 model
    QModelIndex currentLeftIndex; // 左侧学科的树中当前选中节点的 index
    QModelIndex currentKpIndex;   // 右侧知识点树中当前选中节点的 index
    KpService *kpService   = NULL;
    QDir kpsDir; // 教材所在文件夹
};

#endif // KPEDITOR_H
