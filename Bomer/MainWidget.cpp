#include "MainWidget.h"
#include "ui_MainWidget.h"
#include "BomUtils.h"

#include <QFileDialog>
#include <QList>
#include <QDebug>
#include <QDirIterator>
#include <QFileInfo>

MainWidget::MainWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MainWidget) {
    ui->setupUi(this);

    // 选择文件夹
    connect(ui->browseButton, &QPushButton::clicked, [this]() {
        QString dir = QFileDialog::getExistingDirectory(this,
                                                        tr("Open Directory"),
                                                        ".",
                                                        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        ui->dirEdit->setText(dir);
    });

    // 点击 "执行" 按钮执行添加或者删除文件夹及其子文件夹下所有匹配的文件的 BOM
    connect(ui->processButton, &QPushButton::clicked, [this]() {
        QString dir = ui->dirEdit->text().trimmed();
        bool needBom = ui->needBomRadioButton->isChecked();
        QList<QString> suffixes = getSuffixes();

        processBom(dir, suffixes, needBom);
    });
}

MainWidget::~MainWidget() {
    delete ui;
}

QList<QString> MainWidget::getSuffixes() {
    QList<QString> suffixes = ui->suffixesEdit->text().split(",");

    for (int i = 0; i < suffixes.size(); ++i) {
        suffixes[i] = suffixes[i].trimmed().toLower();
    }

    return suffixes;
}

void MainWidget::processBom(const QString &dir, const QList<QString> suffixes, bool needBom) {
    QDirIterator it(dir, QDir::NoDot | QDir::NoDotDot | QDir::Files, QDirIterator::Subdirectories);

    // 遍历文件夹及其子文件夹下所有的文件
    while (it.hasNext()) {
        it.next();

        QFileInfo info = it.fileInfo();
        QString suffix = "." + info.suffix().toLower(); // 文件的后缀名
        QString path   = info.canonicalFilePath();      // 文件的绝对路径

        // 如果文件的后缀名在需要处理 BOM 的文件列表中，则作对应的处理
        if (suffixes.contains(suffix)) {
            if (needBom) {
                ui->statusEdit->append(QString("Insert BOM to '%1'").arg(path));
                BomUtils::insertBom(path);
            } else {
                ui->statusEdit->append(QString("Remove BOM from '%1'").arg(path));
                BomUtils::removeBom(path);
            }
        }
    }
}
