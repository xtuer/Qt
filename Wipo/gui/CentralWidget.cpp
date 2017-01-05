#include "CentralWidget.h"
#include "ui_CentralWidget.h"
#include "util/UiUtil.h"
#include "util/Config.h"
#include "bean/Course.h"
#include "bean/Courseware.h"
#include "dao/CourseDao.h"

#include <QButtonGroup>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QAction>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QIcon>
#include <QDebug>

/*-----------------------------------------------------------------------------|
 |                            CentralWidgetPrivate                             |
 |----------------------------------------------------------------------------*/
class CentralWidgetPrivate {
public:
    QStandardItemModel *courseModel;     // 课程的 model
    QStandardItemModel *coursewareModel; // 课件的 model

    CourseDao *courseDao;         // 课程数据访问对象
    QList<Course> currentCourses; // 当前被选中模块的课程

    CentralWidgetPrivate();
    ~CentralWidgetPrivate();
};

CentralWidgetPrivate::CentralWidgetPrivate() {
    courseModel     = new QStandardItemModel();
    coursewareModel = new QStandardItemModel();
    courseDao       = new CourseDao(Config::getConfigFilePath());
}

CentralWidgetPrivate::~CentralWidgetPrivate() {
    delete courseModel;
    delete coursewareModel;
    delete courseDao;
}

/*-----------------------------------------------------------------------------|
 |                                CentralWidget                                |
 |----------------------------------------------------------------------------*/
CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent), ui(new Ui::CentralWidget), d(new CentralWidgetPrivate) {
    ui->setupUi(this);
    createLoadQssAction();

    ui->courseListView->setModel(d->courseModel);
    ui->coursewareListView->setModel(d->coursewareModel);

    // 把模块的按钮添加到 button group，这样就能做到同时只有一个按钮被选中
    QButtonGroup *bg = new QButtonGroup(this);
    bg->addButton(ui->indexButton);
    bg->addButton(ui->analysisAdvanceButton);
    bg->addButton(ui->analysisBaseButton);
    bg->addButton(ui->analysisInfoButton);
    bg->setExclusive(true);

    connect(ui->courseListView->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &CentralWidget::showCoursewares);
    connect(ui->coursewareListView, &QListView::clicked, this, &CentralWidget::openVideo);
    connect(bg, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(showCourses(QAbstractButton*)));

    ui->indexButton->click(); // 点击第一个按钮
}

CentralWidget::~CentralWidget() {
    delete ui;
    delete d;
}

void CentralWidget::showCourses(QAbstractButton *button) {
    // [1] 当前选中的课程
    if (button == ui->indexButton) {
        d->currentCourses = d->courseDao->indexCourses;
    } else if (button == ui->analysisBaseButton) {
        d->currentCourses = d->courseDao->analysisBaseCourses;
    } else if (button == ui->analysisAdvanceButton) {
        d->currentCourses = d->courseDao->analysisAdvanceCourses;
    } else if (button == ui->analysisInfoButton) {
        d->currentCourses = d->courseDao->analysisInfoCourses;
    }

    // [2] 显示课程
    d->courseModel->clear();
    d->courseModel->setRowCount(d->currentCourses.size());

    for (int i = 0; i < d->currentCourses.size(); ++i) {
        QStandardItem *item = new QStandardItem(d->currentCourses[i].name);
        d->courseModel->setItem(i, item);
    }

    // [3] 选中第一个课程
    d->coursewareModel->clear();
    ui->courseListView->setCurrentIndex(d->courseModel->index(0, 0));
}

void CentralWidget::showCoursewares(const QModelIndex &index) {
    if (!index.isValid()) { return; }
    if (index.row() > d->currentCourses.size()) { return; }

    QList<Courseware> &coursewares = d->currentCourses[index.row()].coursewares;
    d->coursewareModel->clear();
    d->coursewareModel->setRowCount(coursewares.size());

    QIcon playIcon(":/img/play.png");

    for (int i = 0; i < coursewares.size(); ++i) {
        QStandardItem *item = new QStandardItem(playIcon, coursewares[i].name);
        item->setData(coursewares[i].video); // data 为课件的视频路径
        d->coursewareModel->setItem(i, item);
    }
}

void CentralWidget::openVideo(const QModelIndex &index) {
    if (!index.isValid()) { return; }

    QString videoPath = d->coursewareModel->data(index, Qt::UserRole+1).toString();
    bool result = QDesktopServices::openUrl(QUrl::fromLocalFile(videoPath)); // 打开视频

    if (!result) {
        int lastDotIndex = videoPath.lastIndexOf(".");
        QMessageBox::warning(NULL, "错误", QString("打开 '%1' 失败，错误原因:\n1. 文件不存在\n2. 没有安装 %2 播放器")
                             .arg(videoPath)
                             .arg(videoPath.mid(lastDotIndex + 1)));
    }
}

void CentralWidget::createLoadQssAction() {
    QAction *action = new QAction(this);
    action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L)); // 快捷键为 Ctrl + L
    addAction(action);

    connect(action, &QAction::triggered, [] {
        UiUtil::loadQss();
    });
}


