#include "Widget.h"
#include "ui_Widget.h"
#include "util/UiUtil.h"
#include "util/HttpClient.h"
#include "util/Config.h"

#include <QDebug>
#include <QList>
#include <QPair>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFileDialog>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    initialize();
    handleEvents();
}

Widget::~Widget() {
    delete ui;
}

// 初始化
void Widget::initialize() {
    ui->setupUi(this);

    // 设置上传的 URL
    ui->urlLineEdit->setText(ConfigInstance.getUploadUrl());

    // 清除参数 frame 的占位 widget
    UiUtil::emptyWidget(ui->paramsFrame);

    // 增加 stretch
    QBoxLayout *layout = qobject_cast<QBoxLayout *>(ui->paramsFrame->layout());
    if (nullptr != layout) {
        layout->addStretch();
    }
    layout->setSpacing(1);

    // 为了方便使用, 初始化时创建 2 个参数的 widget
    appendParamWidget();
    appendParamWidget();
}

// 信号槽事件处理
void Widget::handleEvents() {
    connect(ui->uploadButton, &QPushButton::clicked, [=] {
        QString url = ui->urlLineEdit->text().trimmed();
        QString file = ui->fileLineEdit->text().trimmed();
        QList<QPair<QString, QString>> params = extractParams();

        // 网址和文件地址校验
        if (url.isEmpty()) {
            QMessageBox::warning(nullptr, "错误", "网址不能为空");
            return;
        }

        if (file.isEmpty()) {
            QMessageBox::warning(nullptr, "错误", "文件地址不能为空");
            return;
        }

        // 上传文件
        HttpClient client = HttpClient(url);
        client.debug(true);

        for (const QPair<QString, QString> &param : params) {
            client.param(param.first, param.second);
        }

        ui->infoEdit->appendPlainText("开始上传……");
        client.upload(file, [this](const QString &response) {
            ui->infoEdit->appendPlainText(response);
            ui->infoEdit->appendPlainText("上传成功");
        }, [this](const QString &error) {
            ui->infoEdit->appendPlainText("上传错误: " + error);
        });
    });

    // 选择上传的文件
    connect(ui->browseButton, &QPushButton::clicked, [this] {
        QString path = QFileDialog::getOpenFileName(nullptr, tr("选择文件"));

        if (!path.isEmpty()) {
            ui->fileLineEdit->setText(path);
        }
    });
}

// 创建参数的 widget: key input, value input, delete button, append button
void Widget::appendParamWidget() {
    QLineEdit   *keyEdit      = new QLineEdit();
    QLineEdit   *valueEdit    = new QLineEdit();
    QPushButton *deleteButton = new QPushButton("x");
    QPushButton *appendButton = new QPushButton("+");

    // 给键值的 edit 设置 object name, 方便获取参数的时候使用 findChild 使用 object name 进行查找
    keyEdit->setObjectName("keyEdit");
    valueEdit->setObjectName("valueEdit");
    keyEdit->setPlaceholderText("键");
    valueEdit->setPlaceholderText("值");

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(keyEdit);
    layout->addWidget(valueEdit);
    layout->addWidget(deleteButton);
    layout->addWidget(appendButton);
    layout->setContentsMargins(0, 0, 0, 0);

    QWidget *paramWidget = new QWidget();
    paramWidget->setLayout(layout);
    paramWidget->setObjectName("paramWidget");

    QVBoxLayout *paramsLayout = qobject_cast<QVBoxLayout*>(ui->paramsFrame->layout());
    paramsLayout->insertWidget(paramsLayout->count() - 1, paramWidget);

    // 点击删除按钮把参数的 widget 从界面上删除
    connect(deleteButton, &QPushButton::clicked, [=] {
        if (paramsLayout->count() > 2) {
            paramWidget->hide();
            paramWidget->deleteLater();
        }
    });

    connect(appendButton, &QPushButton::clicked, [=] {
        appendParamWidget();
    });
}

// 获取参数
QList<QPair<QString, QString>> Widget::extractParams() {
    QList<QWidget*> paramWidgets = ui->paramsFrame->findChildren<QWidget*>("paramWidget");
    QList<QPair<QString, QString>> params;

    // 获取参数, 只有键不为空的参数才获取
    for (QWidget *paramWidget : paramWidgets) {
        QLineEdit *keyEdit   = paramWidget->findChild<QLineEdit*>("keyEdit");
        QLineEdit *valueEdit = paramWidget->findChild<QLineEdit*>("valueEdit");

        QString key = keyEdit->text().trimmed();
        QString value = valueEdit->text();

        if (!key.isEmpty()) {
            params.append(qMakePair(key, value));
        }
    }

    return params;
}
