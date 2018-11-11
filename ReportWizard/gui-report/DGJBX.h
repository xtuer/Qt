#ifndef DGJBX_H
#define DGJBX_H

#include <QWidget>
#include "ReportWizardBase.h"

namespace Ui {
class DGJBX;
}

/**
 * @brief 冻干机 / 冰箱的设置
 */
class DGJBX : public QWidget, public ReportWizardBase {
    Q_OBJECT

public:
    explicit DGJBX(QWidget *parent = nullptr);
    ~DGJBX() override;

    void setReportSettings(ReportSettings settings) override;
    ReportSettings getReportSettings() override;

private:
    Ui::DGJBX *ui;
};

#endif // DGJBX_H
