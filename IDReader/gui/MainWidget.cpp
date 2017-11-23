#include "MainWidget.h"
#include "ui_MainWidget.h"
#include "LoginStatusWidget.h"
#include "magic/MagicWindow.h"
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
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QImage>
#include <QPixmap>

/***********************************************************************************************************************
 *                                                  MainWidgetPrivate                                                  *
 *                                                      数据存储的类                                                     *
 **********************************************************************************************************************/
class MainWidgetPrivate {
public:
    MainWidgetPrivate(MainWidget *widget) {
        readerThread = new CardReaderThread();
        networkManager = new QNetworkAccessManager();
        serverUrl = Singleton<ConfigUtil>::getInstance().getServerUrl();
        loginDetailsButton = new QPushButton("详情");
        loginDetailsButton->setFlat(true);

        initServerTime(); // 请求服务器的时间

        camera = new QCamera(); // 摄像头对象
        viewfinder = new QCameraViewfinder(); // 用于实时显示摄像头图像
        imageCapture = new QCameraImageCapture(camera); // 用于截取摄像头图像

        viewfinder->setAttribute(Qt::WA_StyledBackground, true); // 使 viewfinder 能够使用 QSS
        viewfinder->setFixedSize(164, 90);

        delete widget->ui->cameraWidget->layout()->replaceWidget(widget->ui->placeholderLabel, viewfinder);
        delete widget->ui->placeholderLabel;
        camera->setViewfinder(viewfinder); // camera 使用 viewfinder 实时显示图像
        camera->start();
    }

    ~MainWidgetPrivate() {
        readerThread->stop();
        readerThread->wait();
        camera->stop();

        delete readerThread;
        delete networkManager;
        delete camera;
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

    void initServerTime() {
        // timeDiff is client current time minus server current time.
        QString url = serverUrl + Urls::TIMESTAMP;
        HttpClient(url).manager(networkManager).debug(true).get([this](const QString &response) {
            timeDiff = QDateTime::currentMSecsSinceEpoch() - response.toLongLong();
        });
    }

    // 计算服务器的当前时间，单位为毫秒
    qint64 getCurrentTime() {
        return QDateTime::currentMSecsSinceEpoch() - timeDiff;
    }

    CardReaderThread *readerThread;
    QString serverUrl;
    QNetworkAccessManager *networkManager;
    QPushButton *loginDetailsButton;

    QList<Site> sites;
    QList<PeriodUnit> periodUnits;
    QList<Student> students;

    QCamera *camera;
    QCameraViewfinder *viewfinder;
    QCameraImageCapture *imageCapture;

    qint64 timeDiff; // 客户端和服务器的时间差，单位为毫秒
};

/***********************************************************************************************************************
 *                                                     MainWidget                                                      *
 **********************************************************************************************************************/
MainWidget::MainWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MainWidget) {
    ui->setupUi(this);

    setAttribute(Qt::WA_StyledBackground, true);
    d = new MainWidgetPrivate(this);

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

    // 点击拍照
    connect(ui->captureButton, &QPushButton::clicked, [=] {
        d->imageCapture->capture("id-reader.jpg");
    });

    // 照片拍好后，显示预览，保存到文件，然后上传到服务器
    connect(d->imageCapture, &QCameraImageCapture::imageCaptured, [=](int id, const QImage &image) {
        Q_UNUSED(id)
        // 图片名字
        QString pictureName = uploadCameraPictureName();
        if (pictureName.isEmpty()) {
            return;
        }

        showInfo("", true);

        // 显示预览图
        QImage scaledImage = image.scaled(ui->previewLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->previewLabel->setPixmap(QPixmap::fromImage(scaledImage));

        // 保存到本地
        QString path = QString("student-camera-picture/%1").arg(pictureName);
        QImage userImage = image.scaled(500, 500, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        userImage.save(path, "jpg", 50);

        // 上传到服务器
        QString url = d->serverUrl + Urls::UPLOAD_PHOTO;
        HttpClient(url).manager(d->networkManager).debug(true).upload(path, [=](const QString &response) {
            if (response == "true") {
                showInfo(ui->nameLabel->text() + "的照片上传成功", false);
                qDebug() << QString("%1 上传成功").arg(pictureName);
            } else {
                showInfo(response, true);
                qDebug() << response;
            }
        }, [=](const QString &error) {
            showInfo(error, true);
            qDebug() << error;
        });
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
    ui->birthdayLabel->setText(Util::formatDate(p.birthday));
    ui->cardIdLabel->setText(p.cardId);
    ui->addressLabel->setText(p.address);
    ui->policeLabel->setText(p.police);
    ui->validStartLabel->setText(Util::formatDate(p.validStart));
    ui->validEndLabel->setText(Util::formatDate(p.validEnd));
    ui->pictureLabel->setPixmap(QPixmap(CardReader::personImagePath()));
}

QString MainWidget::uploadCameraPictureName() {
    QString siteCode = ui->siteComboBox->currentData().toString();
    QString roomCode = ui->roomComboBox->currentData().toString();
    QString periodUnitCode = ui->periodUnitComboBox->currentData().toString();
    QString cardId = ui->cardIdLabel->text().trimmed();

    if (periodUnitCode.isEmpty()) {
        showInfo("请选择考期", true);
        return "";
    }

    if (siteCode.isEmpty()) {
        showInfo("请选择考点", true);
        return "";
    }

    if (roomCode.isEmpty()) {
        showInfo("请选择考场", true);
        return "";
    }

    if (cardId.isEmpty()) {
        showInfo("请先刷卡", true);
        return "";
    }

    return QString("%1-%2-%3-%4.jpg").arg(periodUnitCode).arg(siteCode).arg(roomCode).arg(cardId);
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
    HttpClient(url).debug(true).manager(d->networkManager).param("siteCode", siteCode)
            .param("roomCode", roomCode).param("periodUnitCode", periodUnitCode)
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
    HttpClient(url).debug(true).manager(d->networkManager).get([this](const QString &jsonResponse) {
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

        showInfo("初始化完成，请选择考期、考点、考场", false);
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

    // 复制身份证图片到 student-id-picture 目录下
    QString studentPicture = QString("student-id-picture/%1.bmp").arg(p.cardId);
    QFile::copy("person.bmp", studentPicture);

    // http://192.168.10.85:8080/signIn/?idCardNo=5225********414&examineeName=黄彪&siteCode=S001
    // &roomCode=001&periodUnitCode=160901&signAt=1232142&sign=asdfasdf123
    QString signAt = QString::number(QDateTime::currentMSecsSinceEpoch() - d->timeDiff);
    QString sign = Util::md5(QString("%1%2").arg(p.cardId).arg(signAt).toUtf8());
    QString url = d->serverUrl + Urls::SIGN_IN;
    HttpClient(url).debug(true).manager(d->networkManager)
            .param("idCardNo", p.cardId)
            .param("examineeName", p.name)
            .param("siteCode", siteCode)
            .param("roomCode", roomCode)
            .param("periodUnitCode", periodUnitCode)
            .param("signAt", signAt)
            .param("sign", sign)
            .upload(studentPicture, [=](const QString &response) {
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
    HttpClient(url).debug(true).manager(d->networkManager).get([this](const QString &jsonResponse) {
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
    MagicWindow *window = new MagicWindow(lsw, QMargins(1, 1, 1, 1), QMargins(3, 3, 3, 3), ":/img/solid.png", true);
    window->setTitle("");
    window->setTitleBarButtonsVisible(false, false, true);
    window->resize(580, 500);
    window->setResizable(false);

    window->setAttribute(Qt::WA_DeleteOnClose);
    window->setWindowModality(Qt::ApplicationModal);

    window->show();
}
