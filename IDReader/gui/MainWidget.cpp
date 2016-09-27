#include "MainWidget.h"
#include "ui_MainWidget.h"
#include "Constants.h"
#include "bean/Person.h"
#include "bean/Student.h"
#include "bean/Room.h"
#include "bean/Site.h"
#include "bean/PeriodUnit.h"
#include "reader/CardReader.h"
#include "reader/ReadThread.h"
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

// 存储数据的类
class MainWidgetPrivate {
public:
    MainWidgetPrivate() {
        readThread = new ReadThread();
        networkManager = new QNetworkAccessManager();
        serverUrl = Singleton<ConfigUtil>::getInstance().getServerUrl();
        loginDetailsButton = new QPushButton("详情");
        loginDetailsButton->setFlat(true);
    }

    ~MainWidgetPrivate() {
        readThread->stop();
        readThread->wait();
        delete readThread;
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

    ReadThread *readThread;
    QString serverUrl;
    QNetworkAccessManager *networkManager;
    QPushButton *loginDetailsButton;

    QList<Site> sites;
    QList<PeriodUnit> periodUnits;
};

MainWidget::MainWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MainWidget) {
    ui->setupUi(this);

    setAttribute(Qt::WA_StyledBackground, true);
    d = new MainWidgetPrivate();

    // 添加详情按钮
    QHBoxLayout *l = new QHBoxLayout();
    l->setContentsMargins(0,0,10,0);
    l->addStretch();
    l->addWidget(d->loginDetailsButton);
    ui->statisticLabel->setLayout(l);

    handleEvents();
}

MainWidget::~MainWidget() {
    delete d;
    delete ui;
}

void MainWidget::showInfo(const QString &info, bool error) {
    ui->infoLabel->setProperty("error", error);
    ui->infoLabel->setText(info);

    UiUtil::updateQss(ui->infoLabel);
}

// 加载考场的学生信息
void MainWidget::loadRoomEnrollment() {
    QString siteCode = ui->siteComboBox->currentData().toString();
    QString roomCode = ui->roomComboBox->currentData().toString();
    QString periodUnitCode = ui->periodUnitComboBox->currentData().toString();

    if (siteCode.isEmpty() || roomCode.isEmpty() || periodUnitCode.isEmpty()) {
        showInfo("siteCode or roomCode or periodUnitCode cannot be empty");
        return;
    }

    // http://192.168.10.85:8080/getRoomEnrollment?siteCode=S001&roomCode=001&periodUnitCode=160901
    QString url = d->serverUrl + Urls::GET_ROOM_ENROLLMENT;
    HttpClient(url).useManager(d->networkManager).addParam("siteCode", siteCode).addParam("roomCode", roomCode).addParam("periodUnitCode", periodUnitCode)
            .get([this](const QString &jsonResponse) {
        qDebug() << jsonResponse;
        JsonReader json(jsonResponse.toUtf8());

        if (1 != json.getInt("status")) {
            showInfo(jsonResponse, true);
            return;
        }
    }, [this](const QString &error) {
        showInfo(error, true);
    });
}

// 加载考试信息
void MainWidget::loadRoomInformation() {
    // http://192.168.10.85:8080/initializeRoom
    QString url = d->serverUrl + Urls::INITIALIZE_ROOM;
    HttpClient(url).useManager(d->networkManager).get([this](const QString &jsonResponse) {
        // 解析 Site
        d->sites = ResponseUtil::responseToSites(jsonResponse);
        ui->siteComboBox->clear();

        foreach (const Site &s, d->sites) {
            ui->siteComboBox->addItem(s.siteName, s.siteCode);
        }

        // 解析 PeriodUnit
        d->periodUnits = ResponseUtil::responseToPeroidUnits(jsonResponse);
        ui->periodUnitComboBox->clear();

        foreach (const PeriodUnit &pu, d->periodUnits) {
            ui->periodUnitComboBox->addItem(pu.periodUnitCode, pu.periodUnitCode);
        }
    }, [this](const QString &error) {
        showInfo(error, true);
    });
}

// 刷卡后发送学生信息到服务器
void MainWidget::signIn(const Person &p) {
    QString siteCode = ui->siteComboBox->currentData().toString();
    QString roomCode = ui->roomComboBox->currentData().toString();
    QString periodUnitCode = ui->periodUnitComboBox->currentData().toString();

    if (siteCode.isEmpty() || roomCode.isEmpty() || periodUnitCode.isEmpty()) {
        showInfo("siteCode or roomCode or periodUnitCode cannot be empty");
        return;
    }

    // http://192.168.10.85:8080/signIn/?idCardNo=5225********414&examineeName=黄彪&siteCode=S001&roomCode=001&periodUnitCode=160901
    QString url = d->serverUrl + Urls::SIGN_IN;
    HttpClient(url).useManager(d->networkManager).addParam("idCardNo", p.cardId).addParam("examineeName", p.name).addParam("siteCode", siteCode)
            .addParam("roomCode", roomCode).addParam("periodUnitCode", periodUnitCode).get([this](const QString &response) {
        JsonReader json(response.toUtf8());

        if (1 != json.getInt("statusCode")) {
            showInfo(response, false);
            return;
        }

        showInfo("刷卡成功");

        // 更新已同步数(+1)
        // 更新未已同步数

    }, [this](const QString &error) {
        showInfo(error, true);
    });
}

void MainWidget::handleEvents() {
    // 加载初始化信息
    connect(ui->loadExamButton, &QPushButton::clicked, [this]() {
        loadRoomInformation();
    });

    // 连接或断开身份证读卡器
    connect(ui->toggleReadButton, &QPushButton::clicked, [this]() {
        if (!d->readThread->isRunning()) {
            d->readThread->start();
            ui->toggleReadButton->setText("结束刷卡");
        } else {
            d->readThread->stop();
            d->readThread->wait();
            ui->toggleReadButton->setText("开始刷卡");
        }
    });

    // 读取到身份证信息
    connect(d->readThread, &ReadThread::personReady, [this](const Person &p) {
        ui->nameLabel->setText(p.name);
        ui->genderLabel->setText(p.gender);
        ui->nationalityLabel->setText(p.nationality);
        ui->birthdayLabel->setText(Util::formatDate(p.birthday));
        ui->cardIdLabel->setText(p.cardId);
        ui->addressLabel->setText(p.address);
        ui->policeLabel->setText(p.police);
        ui->validStartLabel->setText(Util::formatDate(p.validStart));
        ui->validEndLabel->setText(Util::formatDate(p.validEnd));
        ui->pictureLabel->setPixmap(QPixmap(CardReader::personImagePath()));

        // 发送学生信息到服务器: 使用 invokeMethod() 解决不同线程问题，因为当前上下文环境属于 ReadThread，不属于 GUI 线程
        QMetaObject::invokeMethod(this, "signIn", Q_ARG(Person, p));
    });

    // 读卡线程信息
    connect(d->readThread, &ReadThread::info, [this](int code, const QString &message) {
        if (Constants::CODE_READ_READY == code) {
            showInfo(Constants::INFO_READ_READY, false);
        } else if (11 != code){
            // 11 为读卡失败，定时读卡时没有卡
            showInfo(message, true);
        }

        // 端口打开失败
        if (1 == code) {
            ui->toggleReadButton->setText("开始刷卡");
        }
    });

    // 从服务器加载学生刷卡信息
    connect(d->loginDetailsButton, &QPushButton::clicked, [this] {
        loadRoomEnrollment();
    });

    // 当 Site 变化时，重行初始化 Room 信息
    connect(ui->siteComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [this](int index) {
        QString siteCode = ui->siteComboBox->itemData(index).toString();
        Site s = d->findSite(siteCode);

        ui->roomComboBox->clear();
        foreach (const Room &room, s.rooms) {
            ui->roomComboBox->addItem(room.roomCode, room.roomCode);
        }

        ui->roomComboBox->setCurrentIndex(-1);
    });

    // 当 Room 变化时，加载刷卡信息
    connect(ui->roomComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), [this]() {
        loadRoomEnrollment();
    });
}
