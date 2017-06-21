#include "CommentSettingsDialog.h"
#include "ui_CommentSettingsDialog.h"

#include "bean/NameValue.h"
#include "dao/NameValueDao.h"

static const QString NAME_USERS("users");
static const QString NAME_COMMENTS("comments");
static const QString NAME_PLATFORM("platform");
static const QString PRODUCT_PLATFORM("product");

CommentSettingsDialog::CommentSettingsDialog(QWidget *parent) : QDialog(parent), ui(new Ui::CommentSettingsDialog) {
    ui->setupUi(this);
    layout()->setContentsMargins(8, 8, 8, 8);

    // 读取用户、评价
    QString users = NameValueDao::findValueByName(NAME_USERS);
    QString comments = NameValueDao::findValueByName(NAME_COMMENTS);
    QString platform = NameValueDao::findValueByName(NAME_PLATFORM);

    ui->usersEdit->setPlainText(users);
    ui->commentsEdit->setPlainText(comments);

    if (PRODUCT_PLATFORM == platform) {
        ui->productRadioButton->setChecked(true);
    } else {
        ui->testRadioButton->setChecked(true);
    }

    // 保存用户、评价
    connect(ui->saveButton, &QPushButton::clicked, [this] {
        NameValue usersBean;
        usersBean.name = NAME_USERS;
        usersBean.value = ui->usersEdit->toPlainText();

        NameValue commentsBean;
        commentsBean.name = NAME_COMMENTS;
        commentsBean.value = ui->commentsEdit->toPlainText();

        NameValue platformBean;
        platformBean.name = NAME_PLATFORM;
        platformBean.value = ui->productRadioButton->isChecked() ? PRODUCT_PLATFORM : "test";

        NameValueDao::update(usersBean);
        NameValueDao::update(commentsBean);
        NameValueDao::update(platformBean);
    });
}

CommentSettingsDialog::~CommentSettingsDialog() {
    delete ui;
}
