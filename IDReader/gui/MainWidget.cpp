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
        serverUrl = Singleton<ConfigUtil>::getInstance().getServerUrl();
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
    QString serverUrl;
    QNetworkAccessManager *networkManager;
    QPushButton *loginDetailsButton;

    QList<Site> sites;
    QList<PeriodUnit> periodUnits;
    QList<Student> students;
};

/***********************************************************************************************************************
 *                                                     MainWidget                                                      *
 **********************************************************************************************************************/
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

    // 从服务器加载学生刷卡信息
    connect(d->loginDetailsButton, &QPushButton::clicked, [this] {
        showLoginStatusWidget(d->students);
        // mocLoadStudents();
    });

    // 加载考期和考点
    connect(ui->loadExamButton, &QPushButton::clicked, [this]() {
        loadSiteAndPeriodUnit();
    });

    // 当 PeriodUnit 变化时，取消 Room 的选择
    connect(ui->periodUnitComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [this]() {
        ui->roomComboBox->setCurrentIndex(0);
    });

    // 当 Site 变化时，重行初始化 Room 信息
    connect(ui->siteComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [this](int index) {
        QString siteCode = ui->siteComboBox->itemData(index).toString();
        loadRoom(siteCode);
    });

    // 当 Room 变化时，加载学生的刷卡信息
    connect(ui->roomComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [this]() {
        loadStudents();
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

    // 登陆
    login(p);
}

// 加载考场的学生信息
void MainWidget::loadStudents() {
    updateLoginStatistics(QList<Student>()); // 清空登陆统计

    QString siteCode = ui->siteComboBox->currentData().toString();
    QString roomCode = ui->roomComboBox->currentData().toString();
    QString periodUnitCode = ui->periodUnitComboBox->currentData().toString();

    if (siteCode.isEmpty() || roomCode.isEmpty() || periodUnitCode.isEmpty()) {
        // showInfo("siteCode or roomCode or periodUnitCode cannot be empty", true);
        return;
    }

    // http://192.168.10.95:8080/getRoomEnrollment?siteCode=0105013&roomCode=11&periodUnitCode=16090001
    QString url = d->serverUrl + Urls::GET_ROOM_ENROLLMENT;
    HttpClient(url).debug(true).useManager(d->networkManager).addParam("siteCode", siteCode)
            .addParam("roomCode", roomCode).addParam("periodUnitCode", periodUnitCode)
            .get([this](const QString &jsonResponse) {
        JsonReader json(jsonResponse.toUtf8());

        if (1 != json.getInt("status")) {
            showInfo(jsonResponse, true);
            return;
        }

        // 显示学生信息
        d->students = ResponseUtil::responseToStudents(jsonResponse);
        updateLoginStatistics(d->students);
    }, [this](const QString &error) {
        showInfo(error, true);
    });
}

// 加载考期和考点
void MainWidget::loadSiteAndPeriodUnit() {
    // http://192.168.10.85:8080/initializeRoom
    QString url = d->serverUrl + Urls::INITIALIZE_ROOM;
    HttpClient(url).debug(true).useManager(d->networkManager).get([this](const QString &jsonResponse) {
        // 解析考点 Site
        d->sites = ResponseUtil::responseToSites(jsonResponse);
        ui->siteComboBox->clear();
        ui->siteComboBox->addItem("请选择", "");

        foreach (const Site &s, d->sites) {
            ui->siteComboBox->addItem(s.siteName, s.siteCode);
        }

        // 解析考期 PeriodUnit
        d->periodUnits = ResponseUtil::responseToPeroidUnits(jsonResponse);
        ui->periodUnitComboBox->clear();
        ui->periodUnitComboBox->addItem("请选择", "");

        foreach (const PeriodUnit &pu, d->periodUnits) {
            ui->periodUnitComboBox->addItem(pu.period + "-" + pu.unit, pu.periodUnitCode);
        }
    }, [this](const QString &error) {
        showInfo(error, true);
    });
}

// 加载考场，从考点信息里加载
void MainWidget::loadRoom(const QString &siteCode) {
    Site s = d->findSite(siteCode);

    ui->roomComboBox->clear();
    ui->roomComboBox->addItem("请选择", "");
    updateLoginStatistics(QList<Student>());

    foreach (const Room &room, s.rooms) {
        ui->roomComboBox->addItem(room.roomCode, room.roomCode);
    }
}

// 刷卡后发送学生信息到服务器
void MainWidget::login(const Person &p) {
    QString siteCode = ui->siteComboBox->currentData().toString();
    QString roomCode = ui->roomComboBox->currentData().toString();
    QString periodUnitCode = ui->periodUnitComboBox->currentData().toString();

    if (periodUnitCode.isEmpty()) {
        showInfo("请选择考期", true);
        return;
    }

    if (siteCode.isEmpty()) {
        showInfo("请选择考点", true);
        return;
    }

    if (roomCode.isEmpty()) {
        showInfo("请选择考场", true);
        return;
    }

    // http://192.168.10.85:8080/signIn/?idCardNo=5225********414&examineeName=黄彪&siteCode=S001
    // &roomCode=001&periodUnitCode=160901
    QString url = d->serverUrl + Urls::SIGN_IN;
    HttpClient(url).debug(true).useManager(d->networkManager).addParam("idCardNo", p.cardId)
            .addParam("examineeName", p.name).addParam("siteCode", siteCode).addParam("roomCode", roomCode)
            .addParam("periodUnitCode", periodUnitCode)
            .addFormHeader().post([=](const QString &response) {
        JsonReader json(response.toUtf8());

        if (1 != json.getInt("statusCode")) {
            showInfo(json.getString("message"), true);
            return;
        }

        showInfo("刷卡成功");
        loginSuccess(p.cardId);
    }, [this](const QString &error) {
        showInfo(error, true);
    });
}

// idCardNo 的学生登陆成功
void MainWidget::loginSuccess(const QString &idCardNo) {
    int index = d->studentIndex(idCardNo);

    if (-1 == index) {
        return;
    }

    // 如果为空，则说明是第一次刷卡，不是重复刷卡
    if (d->students[index].signedAt.isEmpty()) {
        d->students[index].signedAt = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        updateLoginStatistics(d->students);
    } else {
        d->students[index].signedAt = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    }
}

// 更新刷卡统计
void MainWidget::updateLoginStatistics(const QList<Student> &students) {
    int totalCount = students.size();
    int unloginCount = 0; // 未登陆人数

    foreach (const Student &s, students) {
        if (s.signedAt.isEmpty()) {
            unloginCount++;
        }
    }

    ui->loginCountLabel->setText(QString::number(totalCount - unloginCount));
    ui->unloginCountLabel->setText(QString::number(unloginCount));
    ui->totalCountLabel->setText(QString::number(totalCount));
}

void MainWidget::mocLoadStudents() {
    // http://192.168.10.95:8080/getRoomEnrollment?siteCode=0105013&roomCode=11&periodUnitCode=160900001
    QString url = "http://192.168.10.95:8080/getRoomEnrollment?siteCode=0105013&roomCode=014&periodUnitCode=160900007";
    HttpClient(url).debug(true).useManager(d->networkManager).get([this](const QString &jsonResponse) {
        JsonReader json(jsonResponse.toUtf8());

        if (1 != json.getInt("status")) {
            showInfo(jsonResponse, true);
            return;
        }

        // 显示学生信息
        d->students = ResponseUtil::responseToStudents(jsonResponse);
        showLoginStatusWidget(d->students);
    }, [this](const QString &error) {
        showInfo(error, true);
    });
}

void MainWidget::showLoginStatusWidget(const QList<Student> &students) {
    LoginStatusWidget *lsw = new LoginStatusWidget();
    lsw->setStudents(students);
    FramelessWindow *window = new FramelessWindow(lsw);
    window->setTitleBarButtonsVisible(false, false, true);
    window->resize(580, 500);
    window->setResizable(false);

    window->setAttribute(Qt::WA_DeleteOnClose);
    window->setWindowModality(Qt::ApplicationModal);

    window->show();
}
