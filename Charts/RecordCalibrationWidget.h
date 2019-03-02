#ifndef RECORDCALIBRATIONWIDGET_H
#define RECORDCALIBRATIONWIDGET_H

#include <QWidget>
#include <QList>

struct CalibrationRange;

namespace Ui {
class RecordCalibrationWidget;
}

class RecordCalibrationWidget : public QWidget {
    Q_OBJECT

public:
    explicit RecordCalibrationWidget(QList<CalibrationRange> calibrationRanges, QWidget *parent = nullptr);
    ~RecordCalibrationWidget();

private:
    Ui::RecordCalibrationWidget *ui;
};

#endif // RECORDCALIBRATIONWIDGET_H
