#include "CommentWidget.h"
#include "ui_CommentWidget.h"
#include "gui/CommentSettingsDialog.h"
#include "dao/NameValueDao.h"
#include "bean/Comment.h"
#include "util/Config.h"

#include <QDebug>
#include <QRegularExpression>
#include <QtGlobal>
#include <algorithm>
#include "util/HttpClient.h"
#include <QMutexLocker>

static const QString NAME_USERS("users");
static const QString NAME_COMMENTS("comments");
static const QString NAME_PLATFORM("platform");
static const QString PRODUCT_PLATFORM("product");

CommentWidget::CommentWidget(QWidget *parent) : QWidget(parent), ui(new Ui::CommentWidget) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true);
    layout()->setContentsMargins(8, 8, 8, 8);
    qsrand(QDateTime::currentDateTime().toTime_t());
    networkAccessManager = new QNetworkAccessManager();

    connect(ui->setupButton, &QPushButton::clicked, [this] {
        CommentSettingsDialog dlg;
        dlg.exec();
        loadSettings();
    });

    connect(ui->countSlider, &QSlider::valueChanged, [this] (int value) {
        ui->countLabel->setText(QString::number(value));
    });

    connect(ui->startButton, &QPushButton::clicked, [this] {
        if (ui->productIdEdit->text().trimmed().isEmpty()) {
            showStatus("请输入产品 ID");
            return;
        }

        if (users.size() < 1) {
            showStatus("没有用户");
            return;
        }

        QStringList customComments = loadCustomComments();
        if (defaultComments.size() + customComments.size() < 1) {
            showStatus("没有评价");
            return;
        }

        if (ui->startDateEdit->date() > ui->endDateEdit->date()) {
            showStatus("开始时间不能大于结束时间");
            return;
        }

        submitComments();
    });

    loadSettings();
}

CommentWidget::~CommentWidget() {
    delete ui;
    delete networkAccessManager;
}

void CommentWidget::loadSettings() {
    qDebug() << "load settings";

    users = loadUsers();
    defaultComments = loadDefaultComments();
    isProductPlatform = NameValueDao::findValueByName(NAME_PLATFORM) == PRODUCT_PLATFORM;

    qDebug() << users;
    qDebug() << defaultComments;
    qDebug() << isProductPlatform;
}

QStringList CommentWidget::loadUsers() {
    QStringList users;
    QString usersString = NameValueDao::findValueByName(NAME_USERS);
    QStringList lines = usersString.split(QRegularExpression("\\n"));

    foreach (QString line, lines) {
        line = line.trimmed();
        if (!line.isEmpty()) {
            users.append(line);
        }
    }

    return users;
}

QStringList CommentWidget::loadDefaultComments() {
    // 中性评价
    QStringList defaultComments;
    QString commentsString = NameValueDao::findValueByName(NAME_COMMENTS);
    QStringList lines = commentsString.split(QRegularExpression("\\n"));

    foreach (QString line, lines) {
        line = line.trimmed();
        if (!line.isEmpty()) {
            defaultComments.append(line);
        }
    }

    return defaultComments;
}

QStringList CommentWidget::loadCustomComments() {
    // 自定义评价
    QStringList customComments;
    QString customCommentsString = ui->customCommentsEdit->toPlainText();
    QStringList lines = customCommentsString.split(QRegularExpression("\\n"));

    foreach (QString line, lines) {
        line = line.trimmed();
        if (!line.isEmpty()) {
            customComments.append(line);
        }
    }

    return customComments;
}

void CommentWidget::showStatus(const QString status) {
    ui->statusLabel->setText("状态: " + status);
}

void CommentWidget::shuffleList(QStringList &strs) {
    int length = strs.size();

    for (int i = 0; i < length * 3; ++i) {
        int pos1 = qrand() % length;
        int pos2 = qrand() % length;

        QString temp = strs[pos1];
        strs[pos1] = strs[pos2];
        strs[pos2] = temp;
    }
}

QList<QDate> CommentWidget::generateDates() const {
    // 在开始时间和结束时间中生成 commentCount 个时间
    QList<QDate> dates;
    QDate startDate = ui->startDateEdit->date();
    QDate endDate = ui->endDateEdit->date();
    int days = startDate.daysTo(endDate);
    int commentCount = ui->countSlider->value(); // 评价数量

    if (days < 0) {
        return dates;
    }

    for (int i = 0; i < commentCount; ++i) {
        QDate d = startDate.addDays(qrand() % (days + 1));
        dates.append(d);
    }

    // 传入 sort 需要的比较函数，进行升序排序
    std::sort(dates.begin(), dates.end(), [](QDate a, QDate b) -> bool {
        return a < b;
    });

    return dates;
}

QList<Comment> CommentWidget::generateComments() {
    QStringList comments = loadCustomComments(); // 最后需要提交的评价
    QList<QDate> dates = generateDates();
    QStringList users = this->users;
    int commentCount = ui->countSlider->value(); // 评价数量

    // 挑选 commentCount 个评价
    int len = commentCount - comments.size();
    for (int i = 0; i < len; ++i) {
        comments << defaultComments.at(qrand() % defaultComments.size());
    }

    QList<Comment> result;

    for (int i = 0; i < dates.size() && i < comments.size(); ++i) {
        QString userId = users.at(qrand() % users.size());
        QString content = comments.at(i);
        qulonglong time = (qulonglong)QDateTime(dates.at(i)).toTime_t() * 1000L;

        Comment comment;
        comment.userId = userId;
        comment.content = content;
        comment.time = time;

        result << comment;
    }

    return result;
}

void CommentWidget::submitComments() {
    QList<Comment> comments = generateComments();

    showStatus("开始……");
    ui->startButton->setEnabled(false);
    int productId = ui->productIdEdit->text().trimmed().toInt();

    finishedCount = 0;
    totalCount = comments.size();

    QString url = QString("http://%1/sale/public/comments/innerInsertComment").arg(ConfigInstance.getServerUrl());

    foreach (Comment comment, comments) {
        HttpClient(url).useManager(networkAccessManager).debug(true)
                .addFormHeader()
                .addParam("productId", QString::number(productId))
                .addParam("secret", "1D4D0F00-4E64-450E-A2C1-D7BCAA623A81")
                .addParam("time", QString::number(comment.time))
                .addParam("userId", comment.userId)
                .addParam("content", comment.content)
                .post([this](const QString &response) {
            qDebug() << response;
            oneRequestFinish();
        }, [this](const QString &error) {
            Q_UNUSED(error)
            showStatus(error);
            oneRequestFinish();
        });
    }
}

void CommentWidget::oneRequestFinish() {
    QMutexLocker locker(&mutex);
    ++finishedCount;

    if (finishedCount >= totalCount) {
        showStatus("完成");
        ui->startButton->setEnabled(true);
    }
}



