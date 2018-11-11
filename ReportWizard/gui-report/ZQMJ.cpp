#include "ZQMJ.h"
#include "ui_ZQMJ.h"

#include "bean/ReportSettings.h"

ZQMJ::ZQMJ(QWidget *parent) : QWidget(parent), ui(new Ui::ZQMJ) {
    ui->setupUi(this);
    ReportWizardBase::initialize(ui->stackedWidget);
}

ZQMJ::~ZQMJ() {
    delete ui;
}

void ZQMJ::setReportSettings(ReportSettings settings) {

}

ReportSettings ZQMJ::getReportSettings() {

}
