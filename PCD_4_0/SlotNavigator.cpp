#include "SlotNavigator.h"
#include "ui_SlotNavigator.h"
#include "SlotHome.h"
#include "SlotParams.h"
#include "SlotWarning.h"
#include "SlotHistory.h"
#include "UiUtil.h"

#include <QDebug>

SlotNavigator::SlotNavigator(QWidget *parent): QWidget(parent), ui(new Ui::SlotNavigator) {
    initUi();
    handleEvents();

    // 设置 SlotHome 为当前页。
    UiUtil::setCurrentWidgetOfStackedWidget(slotHome, ui->stackedWidget);

    // 有警告时设置警告为当前页。
    // UiUtil::setCurrentWidgetOfStackedWidget(slotWarning, ui->stackedWidget);
}

SlotNavigator::~SlotNavigator() {
    delete ui;
}

void SlotNavigator::initUi() {
    ui->setupUi(this);

    slotHome    = new SlotHome();
    slotParams  = new SlotParams();
    slotWarning = new SlotWarning();
    slotHistory = new SlotHistory();

    UiUtil::addWidgetIntoStackedWidget(slotHome, ui->stackedWidget);
    UiUtil::addWidgetIntoStackedWidget(slotParams, ui->stackedWidget);
    UiUtil::addWidgetIntoStackedWidget(slotWarning, ui->stackedWidget);
    UiUtil::addWidgetIntoStackedWidget(slotHistory, ui->stackedWidget);

    UiUtil::setWidgetPaddingAndSpacing(slotHome, 0, 0);
    UiUtil::setWidgetPaddingAndSpacing(slotParams, 0, 0);
    UiUtil::setWidgetPaddingAndSpacing(slotWarning, 0, 0);
    UiUtil::setWidgetPaddingAndSpacing(slotHistory, 0, 0);
}

void SlotNavigator::handleEvents() {
    /*
     * 在 Navigator 中切换显示具体每个 slot 对应的数据。
     */

    // 显示编辑灭菌参数界面。
    connect(slotHome, &SlotHome::emitEditSterilizationParams, this, [this] {
        slotParams->setParams("");
        UiUtil::setCurrentWidgetOfStackedWidget(slotParams, ui->stackedWidget);
    });

    // 显示查看本次结果。
    connect(slotHome, &SlotHome::emitCurrentResult, this, [this] {
        // TODO: 设置 slotParams 里要现实的内容。
        slotParams->setParams("本次灭菌参数");
        UiUtil::setCurrentWidgetOfStackedWidget(slotParams, ui->stackedWidget);
    });

    // 显示查看历史结果。
    connect(slotHome, &SlotHome::emitHistoryResult,this,  [this] {
        UiUtil::setCurrentWidgetOfStackedWidget(slotHistory, ui->stackedWidget);
    });

    // 显示主页。
    connect(slotParams, &SlotParams::emitBack, this, [this] {
        UiUtil::setCurrentWidgetOfStackedWidget(slotHome, ui->stackedWidget);
    });
    // 显示主页。
    connect(slotWarning, &SlotWarning::emitBack, this, [this] {
        UiUtil::setCurrentWidgetOfStackedWidget(slotHome, ui->stackedWidget);
    });
    // 显示主页。
    connect(slotHistory, &SlotHistory::emitBack, this, [this] {
        UiUtil::setCurrentWidgetOfStackedWidget(slotHome, ui->stackedWidget);
    });
}
