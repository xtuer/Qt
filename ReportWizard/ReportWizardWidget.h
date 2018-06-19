#ifndef REPORTWIZARDWIDGET_H
#define REPORTWIZARDWIDGET_H

#include "ReportSettings.h"
#include <QWidget>

class ReportSettings;

namespace Ui {
class ReportWizardWidget;
}

/**
 * 报告设置向导
 */
class ReportWizardWidget : public QWidget {
    Q_OBJECT

public:
    explicit ReportWizardWidget(QWidget *parent = 0);
    ~ReportWizardWidget();

signals:
    void reportSettingsChanged(ReportSettings settings);

private:
    void initialize();   // 初始化
    void handleEvents(); // 信号槽事件处理
    ReportSettings extractReportSettings() const; // 获取报表设置

    // 如果 rule 为 0，返回的字符串以 (可) 开头，否则以 (必) 开头
    QString textWithRule(const QString &text, int rule) const;

    Ui::ReportWizardWidget *ui;
};

#endif // REPORTWIZARDWIDGET_H
