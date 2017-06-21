#include "CentralWidget.h"
#include "ui_CentralWidget.h"
#include "util/UiUtil.h"
#include "gui/BuyLoveWidget.h"
#include "gui/CommentWidget.h"

CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent), ui(new Ui::CentralWidget) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true);

    initialize();
}

CentralWidget::~CentralWidget() {
    delete ui;
}

void CentralWidget::initialize() {
    createLoadQssAction();

    // 删除 widgets 之间的 spaing.
    // 删除 widgets 和边框之间的 margin.
    layout()->setSpacing(0);
    layout()->setContentsMargins(0, 0, 0, 0);

    connect(ui->buttonGroup, SIGNAL(buttonClicked(QAbstractButton *)), this, SLOT(changeCurrentWidget(QAbstractButton *)));
    ui->commentButton->click(); // 显示第一个 widget
}

void CentralWidget::createLoadQssAction() {
    QAction *action = new QAction(this);
    action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
    addAction(action);
    connect(action, &QAction::triggered, [] {
        UiUtil::loadQss();
    });
}

/**
 * 按下侧边栏的按钮后显示与之相对应的 widget
 * @param button
 */
void CentralWidget::changeCurrentWidget(QAbstractButton *button) {
    // 动态加载对应的widget
    if (!buttonWidgetHash.contains(button)) {
        createRelatedWidget(button);
    }

    QWidget *targetWidget = buttonWidgetHash.value(button);
    for (; NULL != targetWidget; targetWidget = targetWidget->parentWidget()) {
        if (targetWidget->parentWidget() == ui->contentStackedWidget) {
            ui->contentStackedWidget->setCurrentWidget(targetWidget);
            break;
        }
    }
}

/**
 * 当按钮按下时, 动态创建对应的 widget, 防止程序启动时加载所有的组件
 * 右侧界面中与侧栏按钮对应的组件在此函数中进行添加
 */
void CentralWidget::createRelatedWidget(QAbstractButton *button) {
    if (button == ui->buyLoveButton) {
        // [1] 创建 widget
        QWidget *w = new BuyLoveWidget();
        buttonWidgetHash.insert(ui->buyLoveButton, w);

        // [2] 添加 widget 到窗口中
        UiUtil::addWidgetIntoStackedWidget(w, ui->contentStackedWidget);
    } else if (button == ui->commentButton) {
        QWidget *w = new CommentWidget();
        buttonWidgetHash.insert(ui->commentButton, w);

        UiUtil::addWidgetIntoStackedWidget(w, ui->contentStackedWidget);
    }
}
