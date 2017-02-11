#include "MainWidget.h"
#include "ui_MainWidget.h"
#include "util/ConfigUtil.h"
#include "util/UiUtil.h"
#include "gui/ArrangeCourseWidget.h"
#include "gui/DesignScheduleWidget.h"
#include "gui/ClassCourseTeacherWidget.h"
#include "gui/ClassScheduleWidget.h"
#include "gui/TeacherScheduleWidget.h"

#include<QIcon>

MainWidget::MainWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MainWidget) {
    ui->setupUi(this);
    initializeWidgets();
    createLoadQssAction();
}

MainWidget::~MainWidget() {
    delete ui;
}

void MainWidget::initializeWidgets() {
    // Push the buttons in the sidebar in a button group so that only one of them can be checked.
    sidebarButtonGroup = new QButtonGroup(this);
    sidebarButtonGroup->addButton(ui->classScheduleButton);
    sidebarButtonGroup->addButton(ui->teacherScheduleButton);
    sidebarButtonGroup->addButton(ui->designScheduleButton);
    sidebarButtonGroup->addButton(ui->arrangeCourseButton);
    sidebarButtonGroup->addButton(ui->classCourseTeacherButton);
    sidebarButtonGroup->setExclusive(true);

    connect(sidebarButtonGroup, SIGNAL(buttonClicked(QAbstractButton *)), this, SLOT(changeCurrentWidget(QAbstractButton *)));
    ui->classScheduleButton->click();
}

void MainWidget::createLoadQssAction() {
    QAction *action = new QAction(this);
    action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
    addAction(action);
    connect(action, &QAction::triggered, []() {
        UiUtil::loadQss();
    });
}

/**
 * 按下侧边栏的按钮后显示与之相对应的 widget
 * @param button
 */
void MainWidget::changeCurrentWidget(QAbstractButton *button) {
    // 动态加载对应的widget
    if (!buttonWidgetHash.contains(button)) {
        createRelatedWidget(button);
    }

    QWidget *targetWidget = buttonWidgetHash.value(button, NULL);
    for (; NULL != targetWidget; targetWidget = targetWidget->parentWidget()) {
        if (targetWidget->parentWidget() == ui->contentStackedWidget) {
            ui->contentStackedWidget->setCurrentWidget(targetWidget);
            break;
        }
    }
}

/**
 * 当按钮按下时, 动态创建对应的widget, 防止程序启动时加载所有的组件
 * -- 界面中与侧栏按钮对应的组件在此函数中进行添加
 */
void MainWidget::createRelatedWidget(QAbstractButton *button) {
    QWidget *w = NULL;

    if (button == ui->classScheduleButton) {
        // 班级课表
        w = new ClassScheduleWidget();
        buttonWidgetHash.insert(ui->classScheduleButton, w);

        UiUtil::addWidgetIntoStackedWidget(w, ui->contentStackedWidget);
    } else if (button == ui->teacherScheduleButton) {
        // 老师课表
        w = new TeacherScheduleWidget();
        buttonWidgetHash.insert(ui->teacherScheduleButton, w);

        UiUtil::addWidgetIntoStackedWidget(w, ui->contentStackedWidget);
    } else if (button == ui->designScheduleButton) {
        // 课表设计
        w = new DesignScheduleWidget();
        buttonWidgetHash.insert(ui->designScheduleButton, w);

        UiUtil::addWidgetIntoStackedWidget(w, ui->contentStackedWidget);
    } else if (button == ui->arrangeCourseButton) {
        // 课程安排
        w = new ArrangeCourseWidget();
        UiUtil::updateQss(w);
        buttonWidgetHash.insert(ui->arrangeCourseButton, w);

        UiUtil::addWidgetIntoStackedWidget(w, ui->contentStackedWidget);
    } else if (button == ui->classCourseTeacherButton) {
        // 班级－课程－老师
        w = new ClassCourseTeacherWidget();
        buttonWidgetHash.insert(ui->classCourseTeacherButton, w);

        UiUtil::addWidgetIntoStackedWidget(w, ui->contentStackedWidget);
    }

    // 设置 Widget 的 padding 和 spacing
    if (NULL != w) {
        int padding = Singleton<ConfigUtil>::getInstance().getLayoutPadding();
        int spacing = Singleton<ConfigUtil>::getInstance().getLayoutSpacing();
        UiUtil::setWidgetPaddingAndSpacing(w, padding, spacing);
    }
}
