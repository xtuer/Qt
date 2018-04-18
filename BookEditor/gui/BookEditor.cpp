#include "BookEditor.h"
#include "ui_BookEditor.h"
#include "util/Json.h"

#include <QDebug>
#include <QFileDialog>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QJsonArray>
#include <QStringList>

BookEditor::BookEditor(QWidget *parent) : QWidget(parent), ui(new Ui::BookEditor) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);

    chapterModel = new QStandardItemModel();
    ui->treeView->setModel(chapterModel);
    chapterModel->setHorizontalHeaderLabels(QStringList() << "章节" << "编码");
    ui->treeView->setColumnWidth(0, 250);

    handleEvents();
}

BookEditor::~BookEditor() {
    delete ui;
}

void BookEditor::handleEvents() {
    // 加载书籍
    connect(ui->openButton, &QPushButton::clicked, [this] {
        QString filename = QFileDialog::getOpenFileName(this, "打开书籍", ".", "书籍 (*.json)");

        if (filename.isEmpty()) {
            return;
        }

        chapterModel->removeRows(0, chapterModel->rowCount());

        Json json(filename, true);
        ui->bookCodeEdit->setText(json.getString("code"));
        ui->bookNameEdit->setText(json.getString("title"));
        ui->bookLogoEdit->setText(json.getString("metadata.logo"));
        ui->bookVersionEdit->setText(json.getString("metadata.version"));
        ui->bookEditionEdit->setText(json.getString("metadata.edition"));

        QJsonArray chapters = json.getJsonArray("chapters");
        for (QJsonArray::const_iterator iter = chapters.begin(); iter != chapters.end(); ++iter) {
            QJsonObject chapter = iter->toObject();
            createChapters(json, chapter, NULL);
        }
    });
}

void BookEditor::createChapters(const Json &json, const QJsonObject &chapter, QStandardItem *parentItem) {
    QString title = json.getString("title", "", chapter);
    QString code = json.getString("code", "", chapter);
    QList<QStandardItem*> childColumns;

    if (NULL == parentItem) {
        parentItem = new QStandardItem(title);
        childColumns << parentItem << new QStandardItem(code);
        chapterModel->appendRow(childColumns);
    } else {
        QStandardItem *item = new QStandardItem(title);
        childColumns << item << new QStandardItem(code);
        parentItem->appendRow(childColumns);
        parentItem = item;
    }

    // 章节下的章节
    QJsonArray subchapters = json.getJsonArray("children", chapter);
    for (QJsonArray::const_iterator iter = subchapters.begin(); iter != subchapters.end(); ++iter) {
        QJsonObject subchapter = iter->toObject();
        createChapters(json, subchapter, parentItem);
    }
}
