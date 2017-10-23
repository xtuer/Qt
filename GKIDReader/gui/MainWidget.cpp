#include "MainWidget.h"
#include "ui_MainWidget.h"
#include "LoginStatusWidget.h"
#include "FramelessWindow.h"
#include "Constants.h"
#include "bean/Person.h"
#include "bean/Student.h"
#include "bean/Room.h"
#include "bean/Site.h"
#include "bean/PeriodUnit.h"
#include "reader/CardReader.h"
#include "reader/CardReaderThread.h"
#include "util/Util.h"
#include "util/UiUtil.h"
#include "util/JsonReader.h"
#include "util/ConfigUtil.h"
#include "util/ResponseUtil.h"
#include "util/HttpClient.h"

#include <QDebug>
#include <QNetworkAccessManager>
#include <QDateTime>
#include <QTextStream>
#include <QThread>
#include <QMetaObject>
#include <QHBoxLayout>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>

/***********************************************************************************************************************
 *                                                  MainWidgetPrivate                                                  *
 *                                                      数据存储的类                                                     *
 **********************************************************************************************************************/
class MainWidgetPrivate {
public:
    MainWidgetPrivate() {
        readerThread = new CardReaderThread();
        networkManager = new QNetworkAccessManager();

        debug = ConfigUtilInstance.isDebug();
        loginUrl = ConfigUtilInstance.getLoginUrl();
        timeServiceUrl = ConfigUtilInstance.getTimeServiceUrl();
        deltaTimeBetweenClientAndServer = 100000000;

        loginDetailsButton = new QPushButton("详情");
        loginDetailsButton->setFlat(true);
    }

    ~MainWidgetPrivate() {
        readerThread->stop();
        readerThread->wait();
        delete readerThread;
        delete networkManager;
    }

    // 使用 siteCode 从 sites 里查找第一个有相同 siteCode 的 site
    Site findSite(const QString &siteCode) const {
        foreach (const Site &s, sites) {
            if (siteCode == s.siteCode) {
                return s;
            }
        }

        return Site();
    }

    // 使用 siteCode 从 sites 里查找第一个有相同 siteCode 的 site 下标
    // 如果找不到则返回 -1
    int siteIndex(const QString &siteCode) const {
        for (int i = 0; i < sites.size(); ++i) {
            if (siteCode == sites[i].siteCode) {
                return i;
            }
        }

        return -1;
    }

    // 使用身份证号查找学生
    // 如果找不到则返回 -1
    int studentIndex(const QString &idCardNo) {
        for (int i = 0; i < students.size(); ++i) {
            if (idCardNo == students[i].idCardNo) {
                return i;
            }
        }

        return -1;
    }

    CardReaderThread *readerThread;
    QString loginUrl;
    QString timeServiceUrl;
    QNetworkAccessManager *networkManager;
    QPushButton *loginDetailsButton;

    QList<Site> sites;
    QList<PeriodUnit> periodUnits;
    QList<Student> students;

    bool debug;
    qint64 deltaTimeBetweenClientAndServer; // 程序启动时客户端和服务器端时间差，单位为秒
};

/***********************************************************************************************************************
 *                                                     MainWidget                                                      *
 **********************************************************************************************************************/
MainWidget::MainWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MainWidget) {
    ui->setupUi(this);

    setAttribute(Qt::WA_StyledBackground, true);
    d = new MainWidgetPrivate();

    // 请求服务器的时间
    HttpClient(d->timeServiceUrl).debug(d->debug).useManager(d->networkManager).get([=](const QString &time) {
        qDebug() << QString("Server Time: %1").arg(time);
        d->deltaTimeBetweenClientAndServer = time.toLongLong() - QDateTime::currentMSecsSinceEpoch() / 1000;
    }, [=](const QString &error) {
        qDebug() << error;
        showInfo(error, true);
    });

    handleEvents();
}

MainWidget::~MainWidget() {
    delete d;
    delete ui;
}

void MainWidget::handleEvents() {
    // 连接或断开身份证读卡器
    connect(ui->toggleReadButton, &QPushButton::clicked, [this]() {
        if (!d->readerThread->isRunning()) {
            d->readerThread->start();
            ui->toggleReadButton->setText("结束刷卡");
        } else {
            d->readerThread->stop();
            d->readerThread->wait();
            ui->toggleReadButton->setText("开始刷卡");
            showInfo("结束刷卡");
        }
    });

    // 读取到身份证信息，Reader 线程
    connect(d->readerThread, &CardReaderThread::personReady, [this](const Person &p) {
        // 显示和发送学生信息到服务器: 使用 invokeMethod() 解决不同线程问题，因为当前上下文环境属于 ReadThread，不属于 GUI 线程
        QMetaObject::invokeMethod(this, "personReady", Q_ARG(Person, p));
    });

    // 读卡线程信息，Reader 线程
    connect(d->readerThread, &CardReaderThread::info, [this](int code, const QString &message) {
        if (Constants::CODE_READ_READY == code) {
            //showInfo(Constants::INFO_READ_READY, false);
            QMetaObject::invokeMethod(this, "showInfo", Q_ARG(QString, Constants::INFO_READ_READY));
        } else if (11 != code){
            // 11 为读卡失败，定时读卡时没有卡
            // showInfo(message, true);
            QMetaObject::invokeMethod(this, "showInfo", Q_ARG(QString, message), Q_ARG(bool, true));
        }

        // 端口打开失败
        if (1 == code) {
            // ui->toggleReadButton->setText("开始刷卡");
            QMetaObject::invokeMethod(this, "setReadButtonText", Q_ARG(QString, QString("开始刷卡")));
        }
    });
}

void MainWidget::showInfo(const QString &info, bool error) {
    ui->infoLabel->setProperty("error", error);
    ui->infoLabel->setText(info);

    UiUtil::updateQss(ui->infoLabel);
}

void MainWidget::setReadButtonText(const QString &text) {
    ui->toggleReadButton->setText(text);
}

void MainWidget::personReady(const Person &p) {
    showPerson(p); // 显示身份证信息
    login(p); // 登陆
}

void MainWidget::showPerson(const Person &p) {
    ui->nameLabel->setText(p.name);
    ui->genderLabel->setText(p.gender);
    ui->nationalityLabel->setText(p.nationality);
    ui->birthdayLabel->setText(Util::formatDateCN(p.birthday));
    ui->cardIdLabel->setText(p.cardId);
    ui->addressLabel->setText(p.address);
    ui->policeLabel->setText(p.police);
    ui->validStartLabel->setText(Util::formatDateCN(p.validStart));
    ui->validEndLabel->setText(Util::formatDateCN(p.validEnd));
    ui->pictureLabel->setPixmap(QPixmap(CardReader::personImagePath()));
}

void MainWidget::login(const Person &p) {
    QString birthday = Util::formatDateISO(p.birthday);
    QString startTime = Util::formatDateISO(p.validStart);
    QString endTime = Util::formatDateISO(p.validEnd);
    QString pointCode = QString::number(33);
    QString pointName("福建");
    qint64  time = QDateTime::currentMSecsSinceEpoch() / 1000 + d->deltaTimeBetweenClientAndServer;
    int type = ui->startRadioButton->isChecked() ? 1 : 2; // 1 是入场，2 是退场

    // md5(md5(cardnum+birth+start_time+end_time+point_code+'mainexam201704cdcard')) //验证串
    //QByteArray token = (p.cardId + birthday + startTime + endTime + pointCode + "mainexam201704cdcard").toUtf8();
    QByteArray token = (p.cardId + QString::number(time) + "mainexam201704cdcard").toUtf8();
    token = Util::md5(Util::md5(token));

    HttpClient(d->loginUrl).debug(d->debug).useManager(d->networkManager).addFormHeader()
            .addParam("name", p.name).addParam("cardnum", p.cardId)
            .addParam("sex", p.gender).addParam("nation", p.nationality)
            .addParam("birth", birthday).addParam("start_time", startTime).addParam("end_time", endTime)
            .addParam("address", p.address).addParam("institution", p.police)
            .addParam("check_time", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
            .addParam("point_code", pointCode).addParam("point_name", pointName)
            .addParam("type", QString::number(type))
            .addParam("time", QString::number(time))
            .addParam("token", token).post([=](const QString &response) {
        qDebug() << response;
        showInfo(response);
    }, [=](const QString &error) {
        qDebug() << error;
        showInfo(error, true);
    });
}

//void MainWidget::login(const Person &p) {
//    QString birthday = Util::formatDateISO(p.birthday);
//    QString startTime = Util::formatDateISO(p.validStart);
//    QString endTime = Util::formatDateISO(p.validEnd);
//    QString pointCode = QString::number(33);
//    QString pointName("aa");

//    // md5(md5(cardnum+birth+start_time+end_time+point_code+'mainexam201704cdcard')) //验证串
//    QByteArray token = (p.cardId + birthday + startTime + endTime + pointCode + "mainexam201704cdcard").toUtf8();
//    token = QCryptographicHash::hash(token, QCryptographicHash::Md5).toHex();
//    token = QCryptographicHash::hash(token, QCryptographicHash::Md5).toHex();

//    HttpClient(d->serverUrl).debug(true).useManager(d->networkManager).addFormHeader()
//            .addParam("name", "aa").addParam("cardnum", p.cardId)
//            .addParam("sex", "aa").addParam("nation", "aa")
//            .addParam("birth", birthday).addParam("start_time", startTime).addParam("end_time", endTime)
//            .addParam("address", "aa").addParam("institution", "aa")
//            .addParam("check_time", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
//            .addParam("point_code", pointCode).addParam("point_name", pointName)
//            .addParam("str", token).post([=](const QString &response) {
//        qDebug() << response;
//    }, [=](const QString &error) {
//        qDebug() << error;
//    });
//}
