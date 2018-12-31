#include "RecordCalibrationWidget.h"
#include "ui_RecordCalibrationWidget.h"
#include "SelectableChartView.h"

#include <QDebug>

RecordCalibrationWidget::RecordCalibrationWidget(QList<CalibrationRange> calibrationRanges, QWidget *parent) :
    QWidget(parent), ui(new Ui::RecordCalibrationWidget) {
    ui->setupUi(this);

    // 输出记录器校准的选区范围数据
    for (const CalibrationRange& range : calibrationRanges) {
        qDebug() << range.minTime << range.maxTime << range.minTemperature << range.maxTemperature;
    }
}

RecordCalibrationWidget::~RecordCalibrationWidget() {
    delete ui;
}
