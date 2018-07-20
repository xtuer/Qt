#ifndef COGNITIONWIDGET_H
#define COGNITIONWIDGET_H

#include <QWidget>
#include <QList>
#include <QModelIndex>
#include <QStandardItemModel>

class Json;
class KpService;

namespace Ui {
class CognitionWidget;
}

/**
 * 认知水平选择 widget
 */
class CognitionWidget : public QWidget {
    Q_OBJECT

public:
    explicit CognitionWidget(QWidget *parent = nullptr);
    ~CognitionWidget();

    void setPhaseNameAndSubjectName(const QString &phaseName, const QString &subjectName); // 设置学段和学科
    bool isSelected() const; // 是否选择了认知水平
    QString getSelectedCognitions() const; // 获取选中的认知水平

protected:
    void showEvent(QShowEvent *event);

private:
    void initialize();   // 初始化
    void handleEvents(); // 事件处理
    void loadCognitions(const QString &phaseName, const QString &subjectName); // 加载 [学段 -> 学科] 的认知水平

    Ui::CognitionWidget *ui = nullptr;
    bool selected           = false;
    Json *cognitionJson     = nullptr;
    QStandardItemModel *cognitionsModel = nullptr; // 认知水平 model
    QString phaseName;
    QString subjectName;
};

#endif // COGNITIONWIDGET_H
