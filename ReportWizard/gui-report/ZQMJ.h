#ifndef ZQMJ_H
#define ZQMJ_H

#include <QWidget>
#include "ReportWizardBase.h"

namespace Ui {
class ZQMJ;
}

/**
 * @brief 蒸汽灭菌设备的设置
 */
class ZQMJ : public QWidget, public ReportWizardBase {
    Q_OBJECT

public:
    explicit ZQMJ(QWidget *parent = nullptr);
    ~ZQMJ() override;

    void setReportSettings(ReportSettings settings) override;
    ReportSettings getReportSettings() override;

private:
    Ui::ZQMJ *ui;
};

#endif // ZQMJ_H
