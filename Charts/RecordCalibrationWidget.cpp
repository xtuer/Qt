#include "RecordCalibrationWidget.h"
#include "ui_RecordCalibrationWidget.h"
#include "SelectableChartView.h"

#include <QDebug>
#include <QChart>

RecordCalibrationWidget::RecordCalibrationWidget(QList<CalibrationRange> calibrationRanges, QWidget *parent) :
    QWidget(parent), ui(new Ui::RecordCalibrationWidget) {
    ui->setupUi(this);

    // 输出记录器校准的选区范围数据
    for (const CalibrationRange& range : calibrationRanges) {
        qDebug() << QString("Chart count: %1, Time: [%2, %3]")
                    .arg(range.chart->series().size())
                    .arg(range.minTime.toString("yyyy-MM-dd HH:mm:ss"))
                    .arg(range.maxTime.toString("yyyy-MM-dd HH:mm:ss"));
    }
}

RecordCalibrationWidget::~RecordCalibrationWidget() {
    delete ui;
}
