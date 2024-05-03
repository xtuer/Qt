#include "CentralWidget.h"
#include "ui_CentralWidget.h"
#include "UiUtil.h"
#include "SlotNavigator.h"

#include <QDebug>
#include <QButtonGroup>

/**
 * @brief 按钮的状态，设置不同的状态按钮会切换按钮的背景。
 */
const char*   STATE        = "state";
const QString STATE_RED    = "red";
const QString STATE_GREEN  = "green";
const QString STATE_YELLOW = "yellow";

CentralWidget::CentralWidget(QWidget *parent): QWidget(parent), ui(new Ui::CentralWidget) {
    initUi();
    handleEvents();

    // 把第一个 slot 按钮切换为当前按钮。
    ui->slot1Btn->click();
    // setSlotButtonState("slot4Btn", "red"); // 演示设置按钮的状态。
}

CentralWidget::~CentralWidget() {
    delete ui;
}

// 设置 slot 按钮的状态，目的是为你更新按钮的样式。
void CentralWidget::setSlotButtonState(const QString buttonObjectName, const QString stateValue) {
    QPushButton *btn = findChild<QPushButton*>(buttonObjectName);
    if (btn != nullptr) {
        btn->setProperty(STATE, stateValue);
    }
}

// 初始化界面。
void CentralWidget::initUi() {
    ui->setupUi(this);

    // 设置窗口的尺寸、去掉标题栏、启用 QSS。
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_StyledBackground);
    resize(1024, 600);

    // 把按钮添加到 button group，并且初始化按钮的状态。
    buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(ui->slot1Btn);
    buttonGroup->addButton(ui->slot2Btn);
    buttonGroup->addButton(ui->slot3Btn);
    buttonGroup->addButton(ui->slot4Btn);
    buttonGroup->setExclusive(true);

    ui->slot1Btn->setProperty(STATE, STATE_RED);
    ui->slot2Btn->setProperty(STATE, STATE_GREEN);
    ui->slot3Btn->setProperty(STATE, STATE_GREEN);
    ui->slot4Btn->setProperty(STATE, STATE_YELLOW);
}

// 事件处理。
void CentralWidget::handleEvents() {
    // 点击 button group 中的按钮时，更新按钮的 QSS。
    connect(buttonGroup, &QButtonGroup::buttonToggled, this, [this](QAbstractButton *slotButton){
        // 更新按钮样式。
        UiUtil::updateQss(slotButton);

        // 对应的 widget 不存在则创建。
        if (!buttonWidgetHash.contains(slotButton)) {
            createWidgetInContentStackedWidget(slotButton);
        }

        // 显示对应的 widget。
        QWidget *targetWidget = buttonWidgetHash.value(slotButton);
        UiUtil::setCurrentWidgetOfStackedWidget(targetWidget, ui->stackedWidget);
    });
}

// 创建 slot 按钮对应的 widget。
void CentralWidget::createWidgetInContentStackedWidget(QAbstractButton *slotButton) {
    if (slotButton == ui->slot1Btn) {
        QWidget *w = new SlotNavigator(this);
        buttonWidgetHash.insert(ui->slot1Btn, w);
        UiUtil::addWidgetIntoStackedWidget(w, ui->stackedWidget);
        UiUtil::setWidgetPaddingAndSpacing(w, 47, 0);
    } else if (slotButton == ui->slot2Btn) {
        QWidget *w = new SlotNavigator(this);
        buttonWidgetHash.insert(ui->slot2Btn, w);
        UiUtil::addWidgetIntoStackedWidget(w, ui->stackedWidget);
        UiUtil::setWidgetPaddingAndSpacing(w, 47, 0);
    } else if (slotButton == ui->slot3Btn) {
        QWidget *w = new SlotNavigator(this);
        buttonWidgetHash.insert(ui->slot3Btn, w);
        UiUtil::addWidgetIntoStackedWidget(w, ui->stackedWidget);
        UiUtil::setWidgetPaddingAndSpacing(w, 47, 0);
    } else if (slotButton == ui->slot4Btn) {
        QWidget *w = new SlotNavigator(this);
        buttonWidgetHash.insert(ui->slot4Btn, w);
        UiUtil::addWidgetIntoStackedWidget(w, ui->stackedWidget);
        UiUtil::setWidgetPaddingAndSpacing(w, 47, 0);
    }
}
