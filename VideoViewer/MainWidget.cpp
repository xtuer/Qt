#include "MainWidget.h"
#include "ui_MainWidget.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QUrl>
#include <QIcon>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>

MainWidget::MainWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MainWidget) {
    ui->setupUi(this);
    readConfig();
    showCourses();

    connect(ui->courseListWidget, SIGNAL(currentTextChanged(QString)), this, SLOT(showChapters(QString)));
    connect(ui->chapterListWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(openVideo(QModelIndex)));
}

MainWidget::~MainWidget() {
    delete ui;
}

void MainWidget::showCourses() {
    foreach (const QJsonValue &course, courses) {
        QJsonObject obj = course.toObject();
        ui->courseListWidget->addItem(obj["courseName"].toString());
    }
}

void MainWidget::showChapters(const QString &courseName) {
    foreach (const QJsonValue &course, courses) {
        QJsonObject obj = course.toObject();
        QString cn = course.toObject()["courseName"].toString();

        // 如果不是传入进来的课程，继续查找，直到找到
        if (cn != courseName) {
            continue;
        }

        ui->chapterListWidget->clear();
        QJsonArray chapters = obj["chapters"].toArray();

        foreach (const QJsonValue &chapter, chapters) {
            QJsonObject c = chapter.toObject();
            QString name = c["name"].toString();
            QString videoPath = c["videoPath"].toString();
            QListWidgetItem *item = new QListWidgetItem(QIcon("mp4.png"), name, ui->chapterListWidget);
            item->setData(Qt::UserRole, videoPath);
        }
    }
}

void MainWidget::openVideo(const QModelIndex &index) {
    QString path = index.data(Qt::UserRole).toString();
    bool result = QDesktopServices::openUrl(QUrl::fromLocalFile(path));
    int lastDotIndex = path.lastIndexOf(".");

    if (!result) {
        QMessageBox::warning(NULL, "错误", QString("打开 '%1' 失败，错误原因:\n1. 文件不存在\n2. 还没有安装 %2 播放器")
                             .arg(path)
                             .arg(path.mid(lastDotIndex + 1)));
    }
}

void MainWidget::readConfig() {
    QFile file("config.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(NULL, "错误", "找不到配置文件 config.json");
        exit(1);
    }

    QString config = file.readAll();
    file.close();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(config.toUtf8(), &error);
    qDebug() << error.errorString();
    qDebug() << error.offset;
    courses = doc.array();
}

