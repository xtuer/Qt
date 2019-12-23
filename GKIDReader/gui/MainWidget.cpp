#include "MainWidget.h"
#include "ui_MainWidget.h"
#include "ManualSignDialog.h"
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

        debug = ConfigUtilInstance.isDebug();
        loginUrl = ConfigUtilInstance.getLoginUrl();
        cameraUrl = ConfigUtilInstance.getCameraUrl();
        timeServiceUrl = ConfigUtilInstance.getTimeServiceUrl();
        schoolUrl = ConfigUtilInstance.getSchoolUrl();
        manualSignUrl = ConfigUtilInstance.getManualSignUrl();
        deltaTimeBetweenClientAndServer = 100000000;

        loginDetailsButton = new QPushButton("详情");
        loginDetailsButton->setFlat(true);

        camera = new QCamera(); // 摄像头对象
        viewfinder = new QCameraViewfinder(); // 用于实时显示摄像头图像
        imageCapture = new QCameraImageCapture(camera); // 用于截取摄像头图像

        viewfinder->setAttribute(Qt::WA_StyledBackground, true); // 使 viewfinder 能够使用 QSS
        viewfinder->setFixedSize(164, 90);

        delete widget->ui->cameraWidget->layout()->replaceWidget(widget->ui->placeholderLabel, viewfinder);
        delete widget->ui->placeholderLabel;
        camera->setViewfinder(viewfinder); // camera 使用 viewfinder 实时显示图像

        // 摄像头状态
        QObject::connect(camera, &QCamera::statusChanged, [](QCamera::Status status) {
            qDebug() << "Camera status: " << status;
        });

        QObject::connect(camera, QOverload<QCamera::Error>::of(&QCamera::error), [=](QCamera::Error error) {
            qDebug() << "Camera error: " << error;
        });

        // New Request: 去掉摄像头
        // camera->start(); // 启动摄像头
        widget->ui->cameraWidget->hide();
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

    CardReaderThread *readerThread;
    QString loginUrl;
    QString cameraUrl;
    QString timeServiceUrl;
    QString schoolUrl;
    QString manualSignUrl;
    QNetworkAccessManager *networkManager;
    QPushButton *loginDetailsButton;

    QList<Site> sites;
    QList<PeriodUnit> periodUnits;
    QList<Student> students;

    bool debug;
    qint64 deltaTimeBetweenClientAndServer; // 程序启动时客户端和服务器端时间差，单位为秒

    QCamera *camera;
    QCameraViewfinder *viewfinder;
    QCameraImageCapture *imageCapture;
};

/***********************************************************************************************************************
 *                                                     MainWidget                                                      *
 **********************************************************************************************************************/
MainWidget::MainWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MainWidget) {
    ui->setupUi(this);

    setAttribute(Qt::WA_StyledBackground, true);
    d = new MainWidgetPrivate(this);

    // 请求服务器的时间
    HttpClient(d->timeServiceUrl).debug(d->debug).manager(d->networkManager).get([=](const QString &time) {
        qDebug() << QString("Server Time: %1").arg(time);
        d->deltaTimeBetweenClientAndServer = time.toLongLong() - QDateTime::currentMSecsSinceEpoch() / 1000;
    }, [=](const QString &error) {
        qDebug() << error;
        showInfo(error, true);
    });

    // 请求学校
    HttpClient(d->schoolUrl).debug(d->debug).manager(d->networkManager).get([=] (const QString &response) {
        JsonReader json(response.toUtf8());

        if (json.getBool("success")) {
            QJsonArray schools = json.getJsonValue("data").toArray();
            for (QJsonArray::const_iterator iter = schools.constBegin(); iter != schools.constEnd(); ++iter) {
                QJsonObject school = iter->toObject();
                QString schoolId   = school.value("SCHOOL_ID").toString();
                QString schoolName = school.value("SCHOOL_NAME").toString();

                ui->schoolsComboBox->addItem(schoolName, schoolId);
            }
        } else {
            showInfo("没有学校", true);
        }
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

    // 点击拍照
    connect(ui->captureButton, &QPushButton::clicked, [=] {
        d->imageCapture->capture("id-reader.jpg");
    });

    // 照片拍好后，显示预览，保存到文件，然后上传到服务器
    connect(d->imageCapture, &QCameraImageCapture::imageCaptured, [this](int id, const QImage &image) {
        Q_UNUSED(id)
        qint64 time     = QDateTime::currentMSecsSinceEpoch() / 1000 + d->deltaTimeBetweenClientAndServer;
        QString cardNum = ui->cardIdLabel->text();
        if (cardNum.isEmpty()) {
            showInfo("请先刷身份证", true);
            return;
        }

        // 显示预览图
        QImage scaledImage = image.scaled(ui->previewLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->previewLabel->setPixmap(QPixmap::fromImage(scaledImage));

        // 保存到本地
        QString path = QString("student-camera-picture/%1.jpg").arg(cardNum); //uploadCameraPictureName();
        QImage userImage = image.scaled(500, 500, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        userImage.save(path, "jpg", 50);

        // 上传到服务器
        //QString url = d->loginUrl;
        HttpClient(d->cameraUrl).manager(d->networkManager).debug(true)
                .param("cardNum", cardNum)
                .param("time", QString::number(time))
                .upload(path, [=](const QString &response) {
            qDebug() << response;
            showInfo(response);
        }, [=](const QString &error) {
            showInfo(error, true);
            qDebug() << error;
        });
    });

    // 手动签到
    connect(ui->manualSigninButton, &QPushButton::clicked, [this] {
        ManualSignDialog dialog(this);

        if (QDialog::Accepted == dialog.exec()) {
            QString schoolId = ui->schoolsComboBox->currentData().toString();

            HttpClient(d->manualSignUrl).debug(d->debug).manager(d->networkManager)
                    .param("school_id", schoolId)
                    .param("name", dialog.getName())
                    .param("exam_num", dialog.getExamNum())
                    .param("sign_code", dialog.getSignCode())
                    .post([=](const QString &response) {
                qDebug() << response;
                showInfo(response);
            }, [=](const QString &error) {
                qDebug() << error;
                showInfo(error, true);
            });
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
    QString schoolId = ui->schoolsComboBox->currentData().toString();
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

    // 复制身份证图片到 student-id-picture 目录下
    QString studentPicture = QString("student-id-picture/%1.bmp").arg(p.cardId);
    QFile::copy("person.bmp", studentPicture);

    HttpClient(d->loginUrl).debug(d->debug).manager(d->networkManager)
            .param("school_id", schoolId)
            .param("name", p.name).param("cardnum", p.cardId)
            .param("sex", p.gender).param("nation", p.nationality)
            .param("birth", birthday).param("start_time", startTime).param("end_time", endTime)
            .param("address", p.address).param("institution", p.police)
            .param("check_time", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
            .param("point_code", pointCode).param("point_name", pointName)
            .param("sort", QString::number(0))
            .param("type", QString::number(type))
            .param("time", QString::number(time))
            .param("token", token).upload(studentPicture, [=](const QString &response) {
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
