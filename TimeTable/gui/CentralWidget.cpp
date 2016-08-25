#include "CentralWidget.h"
#include "ui_CentralWidget.h"

#include "gui/ClazzTimeTableWidget.h"
#include "gui/TeacherTimeTableWidget.h"
#include "gui/ManageClazzCourseTeacherWidget.h"
#include "gui/DesignTimeTableWidget.h"

#include "util/UiUtil.h"
#include "util/SettingsUtil.h"

#include <QButtonGroup>

CentralWidget::CentralWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CentralWidget) {
    ui->setupUi(this);

    createLoadStyleAction();
    initialize();
    readSettings();
}

CentralWidget::~CentralWidget() {
    writeSettings();
    delete ui;
}

void CentralWidget::readSettings() {
    SettingsUtil::restoreWindowGeometry(this->objectName(), this);
}

void CentralWidget::writeSettings() {
    SettingsUtil::saveWindowGeometry(this->objectName(), this);
}

// *** 侧栏中的按钮在此添加 ***
void CentralWidget::CentralWidget::initialize() {
    connect(ui->buttonGroup, SIGNAL(buttonClicked(QAbstractButton *)), this, SLOT(changeCurrentWidget(QAbstractButton *)));
    ui->clazzTimeTableButton->click();
}

void CentralWidget::changeCurrentWidget(QAbstractButton *button) {
    // 动态加载对应的widget
    if (!buttonWidgetHash.contains(button)) {
        createRelatedWidget(button);
    }

    QWidget *targetWidget = buttonWidgetHash.value(button);
    for (; NULL != targetWidget; targetWidget = targetWidget->parentWidget()) {
        if (targetWidget->parentWidget() == ui->contentsStackedWidget) {
            ui->contentsStackedWidget->setCurrentWidget(targetWidget);
            break;
        }
    }
}

// 当按钮按下时,动态创建对应的widget,防止程序启动时加载所有的组件
// *** 界面中与侧栏按钮对应的组件在此函数中进行添加 ***
void CentralWidget::createRelatedWidget(QAbstractButton *button) {
    if (button == ui->teacherTimeTableButton) {
        TeacherTimeTableWidget *w = new TeacherTimeTableWidget(this);
        buttonWidgetHash.insert(ui->teacherTimeTableButton, w);

        UiUtil::addWidgetIntoStackedWidget(w, ui->contentsStackedWidget);
    } else if (button == ui->clazzTimeTableButton) {
        ClazzTimeTableWidget *w = new ClazzTimeTableWidget(this);
        buttonWidgetHash.insert(ui->clazzTimeTableButton, w);

        UiUtil::addWidgetIntoStackedWidget(w, ui->contentsStackedWidget);
    } else if (button == ui->manageClazzCourseTeacherButton) {
        ManageClazzCourseTeacherWidget *w = new ManageClazzCourseTeacherWidget(this);
        buttonWidgetHash.insert(ui->manageClazzCourseTeacherButton, w);

        UiUtil::addWidgetIntoStackedWidget(w, ui->contentsStackedWidget);
    } else if (button == ui->designTimeTableButton) {
        DesignTimeTableWidget *w = new DesignTimeTableWidget(this);
        buttonWidgetHash.insert(ui->designTimeTableButton, w);

        UiUtil::addWidgetIntoStackedWidget(w, ui->contentsStackedWidget);
    }
}

// 加载style sheet的快捷键
void CentralWidget::createLoadStyleAction() {
    QAction *action = new QAction(this);
    action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
    addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(loadStyleSheet()));
}

void CentralWidget::loadStyleSheet() {
    UiUtil::loadStyleSheet();
}
