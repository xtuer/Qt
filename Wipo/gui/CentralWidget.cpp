#include "CentralWidget.h"
#include "ui_CentralWidget.h"
#include "util/Json.h"
#include "util/UiUtil.h"
#include "bean/Course.h"
#include "bean/Courseware.h"

#include <QJsonArray>
#include <QButtonGroup>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QAction>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QDebug>

/*-----------------------------------------------------------------------------|
 |                            CentralWidgetPrivate                             |
 |----------------------------------------------------------------------------*/
class CentralWidgetPrivate {
public:
    QStandardItemModel *courseModel;     // 课程的 model
    QStandardItemModel *coursewareModel; // 课件的 model

    QList<Course> indexCourses;           // 检索基础模块的课程
    QList<Course> analysisBaseCourses;    // 分析基础模块的课程
    QList<Course> analysisAdvanceCourses; // 分析提高模块的课程
    QList<Course> analysisInfoCourses;    // 信息分析模块的课程
    QList<Course> currentModuleCourses;   // 当前被选中模块的课程

    CentralWidgetPrivate();
    ~CentralWidgetPrivate();
    QList<Course> loadModuleCourses(Json *config, const QString &module); // 加载模块下的课程
};

CentralWidgetPrivate::CentralWidgetPrivate() {
    courseModel     = new QStandardItemModel();
    coursewareModel = new QStandardItemModel();

    // 从文件读取课程到 Json 对象
    Json config("./data/config.json", true);

    // 加载模块对应的课程
    indexCourses           = loadModuleCourses(&config, "检索基础模块");
    analysisBaseCourses    = loadModuleCourses(&config, "分析基础模块");
    analysisAdvanceCourses = loadModuleCourses(&config, "分析提高模块");
    analysisInfoCourses    = loadModuleCourses(&config, "信息分析模块");
}

CentralWidgetPrivate::~CentralWidgetPrivate() {
    delete courseModel;
    delete coursewareModel;
}

QList<Course> CentralWidgetPrivate::loadModuleCourses(Json *json, const QString &module) {
    QList<Course> courses; // 存储课程
    QJsonArray courseModule = json->getJsonArray(module);

    for (QJsonArray::const_iterator cIter = courseModule.begin(); cIter != courseModule.end(); ++cIter) {
        // [1] 加载课程
        Course course;
        QJsonObject c = (*cIter).toObject();
        course.name = json->getString("courseName", "", c); // 取得课程名字

        // [2] 加载课程下的课件
        QJsonArray cws = json->getJsonArray("coursewares", c);
        for (QJsonArray::const_iterator cwsIter = cws.begin(); cwsIter != cws.end(); ++cwsIter) {
            Courseware courseware;
            QJsonObject cw = (*cwsIter).toObject();
            courseware.name = json->getString("coursewareName", "", cw);
            courseware.video = json->getString("video", "", cw);

            course.coursewares << courseware;
        }

        courses << course;
    }

    return courses;
}

/*-----------------------------------------------------------------------------|
 |                                CentralWidget                                |
 |----------------------------------------------------------------------------*/
CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent), ui(new Ui::CentralWidget), d(new CentralWidgetPrivate) {
    ui->setupUi(this);

    ui->courseListView->setModel(d->courseModel);
    ui->coursewareListView->setModel(d->coursewareModel);

    createLoadQssAction();

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
        d->currentModuleCourses = d->indexCourses;
    } else if (button == ui->analysisBaseButton) {
        d->currentModuleCourses = d->analysisBaseCourses;
    } else if (button == ui->analysisAdvanceButton) {
        d->currentModuleCourses = d->analysisAdvanceCourses;
    } else if (button == ui->analysisInfoButton) {
        d->currentModuleCourses = d->analysisInfoCourses;
    }

    // [2] 显示课程
    d->courseModel->clear();
    d->courseModel->setRowCount(d->currentModuleCourses.size());

    for (int i = 0; i < d->currentModuleCourses.size(); ++i) {
        QStandardItem *item = new QStandardItem(d->currentModuleCourses[i].name);
        d->courseModel->setItem(i, item);
    }

    // [3] 选中第一个课程
    d->coursewareModel->clear();
    ui->courseListView->setCurrentIndex(d->courseModel->index(0, 0));
}

void CentralWidget::showCoursewares(const QModelIndex &index) {
    if (!index.isValid()) { return; }
    if (index.row() > d->currentModuleCourses.size()) { return; }

    QList<Courseware> &coursewares = d->currentModuleCourses[index.row()].coursewares;
    d->coursewareModel->clear();
    d->coursewareModel->setRowCount(coursewares.size());

    for (int i = 0; i < coursewares.size(); ++i) {
        QStandardItem *item = new QStandardItem(coursewares[i].name);
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


