#ifndef QUALITYWIDGET_H
#define QUALITYWIDGET_H

#include <QWidget>
#include <QList>
#include <QModelIndex>
#include <QStandardItemModel>

class Json;
class KpService;

namespace Ui {
class QualityWidget;
}

/**
 * 学业质量水平选择 widget
 */
class QualityWidget : public QWidget {
    Q_OBJECT

public:
    explicit QualityWidget(QWidget *parent = nullptr);
    ~QualityWidget();

    void setPhaseNameAndSubjectName(const QString &phaseName, const QString &subjectName); // 设置学段和学科
    bool isSelected() const; // 是否选择了学业质量水平
    QString getSelectedQualities() const; // 获取选中的学业质量水平

protected:
    void showEvent(QShowEvent *event);

private:
    void initialize();   // 初始化
    void handleEvents(); // 事件处理
    void loadQualities(const QString &phaseName, const QString &subjectName); // 加载 [学段 -> 学科] 的学业质量水平
    void mergeCells(int col); // 合并单元格

    Ui::QualityWidget *ui = nullptr;
    bool selected         = false;
    Json *qualityJson     = nullptr;
    QStandardItemModel *qualitiesModel = nullptr; // 学业质量水平 model
    QString phaseName;
    QString subjectName;
};

#endif // QUALITYWIDGET_H
