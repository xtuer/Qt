#include "ui_AiSignWidget.h"
#include "AiSignWidget.h"
#include "InputDialog.h"
#include "SignInStatusWidget.h"
#include "TopWindow.h"
#include "MessageBox.h"

#include "Constants.h"

#include "bean/Unit.h"
#include "bean/Site.h"
#include "bean/Room.h"
#include "bean/Student.h"
#include "bean/Person.h"
#include "bean/SignInInfo.h"

#include "reader/CardReader.h"
#include "reader/CardReaderThread.h"
#include "service/SignInService.h"

#include "util/Util.h"
#include "util/Json.h"
#include "util/UiUtil.h"
#include "util/Config.h"
#include "util/HttpClient.h"
#include "util/ResponseUtil.h"

#include <QDebug>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QNetworkAccessManager>
#include <QPixmap>
#include <QImage>
#include <QMessageBox>

/*-----------------------------------------------------------------------------|
 |                             AiSignWidgetPrivate                             |
 |----------------------------------------------------------------------------*/
struct AiSignWidgetPrivate {
    AiSignWidgetPrivate();
    ~AiSignWidgetPrivate();

    Site findSite(const QString &siteCode) const; // 使用 siteCode 从 sites 里查找第一个有相同 siteCode 的 site

    // 摄像头
    QCamera             *camera;
    QCameraViewfinder   *cameraViewfinder;
    QCameraImageCapture *cameraImageCapture;

    // 考试信息: 考试单元、考点、考场、学生
    QList<Unit>    units;    // 考试单元
    QList<Site>    sites;    // 考点、考点的成员变量里有考场
    QList<Student> students; // 学生

    bool    signInWithFace;  // 使用人脸识别签名
    QString serverUrl;       // 服务器的 URL
    qint64  timeDiff;        // 客户端和服务器的时间差，单位为毫秒
    SignInMode signInMode;   // 签到模式: 手动签到、刷身份证

    CardReaderThread      *readerThread; // 身份证刷卡器
    QNetworkAccessManager *networkManager;
    SignInStatusWidget    *signInStatusWidget;
    TopWindow             *signInStatusTopWindow = nullptr;

    bool debug; // 是否 debug 模式
};

AiSignWidgetPrivate::AiSignWidgetPrivate() {
    debug          = ConfigInstance.isDebug();
    serverUrl      = ConfigInstance.getServerUrl();
    signInWithFace = ConfigInstance.isSignInWithFace();
    networkManager = new QNetworkAccessManager();
    readerThread   = new CardReaderThread();
    signInMode     = SignInMode::SIGN_IN_WITH_FACE;

    signInStatusWidget = new SignInStatusWidget();
    // signInStatusTopWindow = new TopWindow(signInStatusWidget, {0, 0, 0, 0}, {0, 0, 0, 0});
    // signInStatusTopWindow->setTitle("");
    // signInStatusTopWindow->setResizable(false);
    // signInStatusTopWindow->resize(380, 700);
    // signInStatusTopWindow->setTitleBarButtonsVisible(false, false, true);
}

AiSignWidgetPrivate::~AiSignWidgetPrivate() {
    if (signInWithFace) {
        camera->stop();
    }

    readerThread->stop();
    readerThread->wait();

    delete readerThread;
    delete networkManager;
    delete signInStatusTopWindow;
}

// 使用 siteCode 从 sites 里查找第一个有相同 siteCode 的 site
Site AiSignWidgetPrivate::findSite(const QString &siteCode) const {
    foreach (const Site &s, sites) {
        if (siteCode == s.siteCode) {
            return s;
        }
    }

    return Site();
}

/*-----------------------------------------------------------------------------|
 |                                AiSignWidget                                 |
 |----------------------------------------------------------------------------*/
AiSignWidget::AiSignWidget(QWidget *parent) : QWidget(parent), ui(new Ui::AiSignWidget) {
    ui->setupUi(this);
    initialize();
    handleEvents();
}

AiSignWidget::~AiSignWidget() {
    delete d;
    delete ui;
}

// 初始化
void AiSignWidget::initialize() {
    setAttribute(Qt::WA_StyledBackground);
    d = new AiSignWidgetPrivate();
    ui->cameraContainer->layout()->setAlignment(ui->captureAndUploadButton, Qt::AlignHCenter); // 按钮居中
    ui->signInStatusContentContainer->layout()->replaceWidget(ui->signInStatusPlaceholder, d->signInStatusWidget);
    ui->signInStatusPlaceholder->hide();
    ui->signInStatusButton->hide();

    ui->signInManualButton->hide();
    ui->signInModeSimpleButton->hide();
    ui->signInModeWithFaceButton->hide();

    // 状态默认为错误
    updateSystemStatus(ui->cameraStatusLabel, false);
    updateSystemStatus(ui->idCardReaderStatusLabel, false);
    updateSystemStatus(ui->examStatusLabel, false);

    // loadPeriodUnitAndSiteAndRoom(); // 加载服务器考期、考点、考场
    loadExams();         // 加载考试
    loadServerTime();    // 加载服务器的时间
    startIdCardReader(); // 启动身份证刷卡器

    if (d->signInWithFace) {
        // 使用摄像头认证合一签到
        startCamera(); // 启动摄像头
    } else {
        // 不使用摄像头认证合一签到
        // 1. 隐藏摄像头的相关组件
        // 2. 去掉认证合一签到选项
        // 3. 调整组件的位置
        ui->widget_4->hide(); // [1] 隐藏摄像头的相关组件
        ui->label_42->hide();
        ui->facePictureLabel->hide();
        ui->cameraStatusLabel->hide();
        ui->signInModeComboBox->removeItem(0); // [2] 去掉认证合一签到选项
        QGridLayout *gl = qobject_cast<QGridLayout*>(ui->widget_20->layout());
        gl->addWidget(ui->idCardContainer, 0, 1, 2, 2); // [3] 调整组件的位置
    }
}

// 信号槽事件处理
void AiSignWidget::handleEvents() {
    // 切换考试后从服务器加载考试单元、考点、考场
    connect(ui->examComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [this]() {
        QString examCode = ui->examComboBox->currentData().toString();
        // loadExamUnitAndSiteAndRoom(examCode);
        loadUnits(examCode);
        loadSites(examCode);
    });

    // [Camera] 点击拍照上传按钮进行拍照
    connect(ui->captureAndUploadButton, &QPushButton::clicked, [this] {
        d->signInMode = SignInMode::SIGN_IN_WRITTING;
        d->cameraImageCapture->capture("capture.jpg"); // 如果不传入截图文件的路径，则会使用默认的路径，每次截图生成一个图片
    });

    // [Camera] 拍照完成后的槽函数，可以把 image 保存到自己想要的位置
    connect(d->cameraImageCapture, &QCameraImageCapture::imageCaptured, [this](int id, const QImage &image) {
        Q_UNUSED(id)
        // 1. 显示预览图
        // 1.1 预览图显示到右边
        // 1.2 预览图显示到中间，和身份证照片进行比较: 按高度缩放，然后截取中间部分
        // 2. 保存到系统
        // 3. 签到

        // 获取签到的学生信息
        SignInInfo info = getSignInInfo();
        info.signInMode = d->signInMode;
        if (!info.valid) { return; }

        if (SignInMode::SIGN_IN_WRITTING == info.signInMode) {
            {
                // 保存手写笔迹
                image.scaled(600, 600, Qt::KeepAspectRatio, Qt::SmoothTransformation)
                      .save(info.writePicturePath, "jpg", 80);
            }

            // [1.1] 预览图显示到右边: 手写笔记
            QImage rightImage = image.scaled(ui->previewLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            ui->previewLabel->setPixmap(QPixmap::fromImage(rightImage));
            ui->facePictureLabel->setPixmap(QPixmap());

            signIn(info);
        } else if (SignInMode::SIGN_IN_WITH_FACE == info.signInMode) {
            {
                // 保存人脸照片
                image.scaled(600, 600, Qt::KeepAspectRatio, Qt::SmoothTransformation)
                     .save(info.facePicturePath, "jpg", 80);
            }

            // [1.2] 预览图显示到中间，和身份证照片进行比较: 按高度缩放，然后截取中间部分
            QImage centerImage = image.scaledToHeight(ui->facePictureLabel->size().height(), Qt::SmoothTransformation);
            int x = (centerImage.width() - ui->facePictureLabel->width()) / 2;
            int w = ui->facePictureLabel->width();
            int h = ui->facePictureLabel->height();
            ui->facePictureLabel->setPixmap(QPixmap::fromImage(centerImage.copy(x, 0, w, h)));
            ui->previewLabel->setPixmap(QPixmap());
            ui->previewLabel->setText("拍照预览");

            signIn(info);
        }
    });

    // [考试信息] 当 Period 变化时，取消 Room 的选择
    connect(ui->unitComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [this]() {
        ui->roomComboBox->setCurrentIndex(0);
    });

    // [考试信息] 当 Site 变化时，重行初始化 Room 信息
    connect(ui->siteComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index) {
        QString examCode = ui->examComboBox->itemData(ui->examComboBox->currentIndex()).toString();
        QString siteCode = ui->siteComboBox->itemData(index).toString();

        loadRooms(examCode, siteCode);
    });

    // [考试信息] 当 Room 变化时，加载学生的签到信息
    connect(ui->roomComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [this]() {
        loadStudents();
    });

    // [签到] 手动签到
    connect(ui->signInManualButton2, &QPushButton::clicked, [this] {
        SignInInfo info = getSignInInfo(false);
        info.signInMode = SignInMode::SIGN_IN_MANUALLY;

        // 没有选择考期等则返回
        if (!info.valid) {
            return;
        }

        InputDialog dlg(this);
        dlg.setWindowFlags(dlg.windowFlags() ^ Qt::WindowContextHelpButtonHint);
        if (QDialog::Accepted != dlg.exec()) {
            return;
        }

        // 名字和考籍号 (examUid) 通过输入
        info.name     = dlg.getExamineeName();
        info.examUid  = dlg.getExamUid();
        info.password = dlg.getPassword();

        // 人工签到
        SignInService::signInManually(d->serverUrl + Urls::SIGN_IN_MANUAL, info, this, d->networkManager);
    });

    // [身份证刷卡器] 读取到身份证信息，Reader 线程
    connect(d->readerThread, &CardReaderThread::personReady, this, [this](const Person &p) {
        // 显示和发送学生信息到服务器: 使用 invokeMethod() 解决不同线程问题，
        // 因为当前上下文环境属于 ReadThread，不属于 GUI 线程
        QMetaObject::invokeMethod(this, "updateSystemStatus",
                                  Q_ARG(QWidget*, ui->idCardReaderStatusLabel),
                                  Q_ARG(bool, true));
        QMetaObject::invokeMethod(this, "personReady", Q_ARG(Person, p));
    });

    // [身份证刷卡器] 读卡线程信息，Reader 线程
    connect(d->readerThread, &CardReaderThread::info, this, [this](int code, const QString &message) {
        // qDebug() << "----------" << code << message;
        if (Constants::CODE_READ_READY == code) {
            updateSystemStatus(ui->idCardReaderStatusLabel, true);
            // 正确：为 10000 时连接身份证刷卡器成功
            QMetaObject::invokeMethod(this, "updateSystemStatus",
                                      Q_ARG(QWidget*, ui->idCardReaderStatusLabel),
                                      Q_ARG(bool, true));
            QMetaObject::invokeMethod(this, "showInfo", Q_ARG(QString, Constants::INFO_READ_READY));
        } else if (11 == code){
            // 正确：为 11 时则定时读卡，但是没有卡
            QMetaObject::invokeMethod(this, "updateSystemStatus",
                                      Q_ARG(QWidget*, ui->idCardReaderStatusLabel),
                                      Q_ARG(bool, true));
        } else {
            // 错误：其他为错误状态
            // 1 端口打开失败
            QMetaObject::invokeMethod(this, "updateSystemStatus",
                                      Q_ARG(QWidget*, ui->idCardReaderStatusLabel),
                                      Q_ARG(bool, false));
            qDebug() << message;
        }
    });

    // 显示签到状态窗口
    connect(ui->signInStatusButton, &QPushButton::clicked, [this] {
        // d->signInStatusTopWindow->show();
        if (ui->signInStatusContainer->isHidden()) {
            QWidget *window = UiUtil::findWindow(this);
            window->resize(1180, window->height());
            ui->signInStatusContainer->show();
        } else {
            ui->signInStatusContainer->hide();
            QWidget *window = UiUtil::findWindow(this);
            window->resize(850, window->height());
        }
    });

    // 更换考场后更新此考场的学生登陆状态
    connect(this, SIGNAL(studentsReady(QList<Student>)), d->signInStatusWidget, SLOT(setStudents(QList<Student>)));

    // 申请关机密码
    connect(ui->shutdownPasswordButton, &QPushButton::clicked, [this] {
        QString url = d->serverUrl + Urls::CLOSE_PASSWORD;

        HttpClient(url).debug(d->debug).manager(d->networkManager).get([](const QString &jsonResponse) {
            Json json(jsonResponse.toUtf8());
            QMessageBox::information(nullptr, "关机密码", json.getString("data"));
            // MessageBox::message(QString("客户端关机密码: <font color=red>%1</font>").arg(json.getString("data").trimmed()));
        }, [this](const QString &error) {
            showInfo(error, false);
        });
    });
}

// 启动身份证刷卡器
void AiSignWidget::startIdCardReader() {
    // 启动身份证刷卡器线程
    if (!d->readerThread->isRunning()) {
        d->readerThread->start();
    } else {
        // d->readerThread->stop();
        // d->readerThread->wait();
        showInfo("身份证刷卡器已经启动");
    }
}

// 启动摄像头
void AiSignWidget::startCamera() {
    // 1. 创建设置摄像头相关的对象
    // 2. 启动摄像头
    // 3. 增加定位人脸在摄像头图像的 label
    QGridLayout *layout = qobject_cast<QGridLayout *>(ui->cameraContainer->layout());

    // [1] 创建设置摄像头相关的对象
    // [2] 启动摄像头
    d->camera = new QCamera(this);
    d->cameraViewfinder = new QCameraViewfinder(this);
    d->cameraImageCapture = new QCameraImageCapture(d->camera);
    d->cameraViewfinder->setProperty("class", "CameraWidget");
    layout->replaceWidget(ui->cameraPlaceholderLabel, d->cameraViewfinder);
    ui->cameraPlaceholderLabel->hide();
    d->camera->setViewfinder(d->cameraViewfinder);

    connect(d->camera, &QCamera::stateChanged, [] (QCamera::State state) {
        qDebug() << "Camera State" << state;
    });

    // [Camera] 摄像头状态改变
    connect(d->camera, &QCamera::statusChanged, [this] (QCamera::Status status) {
        qDebug() << "Camera Status" << status;

        if (status == QCamera::ActiveStatus) {
            updateSystemStatus(ui->cameraStatusLabel, true);
        }
    });

    connect(d->camera, QOverload<QCamera::Error>::of(&QCamera::error), [this] (QCamera::Error value) {
        qDebug() << "Camera Error" << value;
        if (QCamera::NoError != value) {
            updateSystemStatus(ui->cameraStatusLabel, false);
        }
    });

    d->camera->start();

    // [3] 定位人脸在摄像头图像的 label
    QLabel *facePositionLabel = new QLabel();
    facePositionLabel->setObjectName("facePositionLabel");
    layout->addWidget(facePositionLabel, 0, 0);
    layout->setAlignment(facePositionLabel, Qt::AlignCenter);
}

// 加载考试
void AiSignWidget::loadExams() {
    QString url = d->serverUrl + Urls::EXAM;
    HttpClient(url).debug(d->debug).manager(d->networkManager).get([this](const QString &jsonResponse) {
        Json json(jsonResponse.toUtf8());
        QJsonArray exams = json.getJsonArray(".");

        for (auto e = exams.constBegin(); e != exams.constEnd(); ++e) {
            QString examName = e->toObject().value("examName").toString();
            QString examCode = e->toObject().value("examCode").toString();

            ui->examComboBox->addItem(examName, examCode);
        }
    }, [this](const QString &error) {
        showInfo(error, true);
    });
}

// 加载考试下的单元
void AiSignWidget::loadUnits(const QString &examCode) {
    if (examCode.isEmpty()) { return; }

    QString url = d->serverUrl + Urls::EXAM_UNITS;
    HttpClient(url).debug(d->debug).manager(d->networkManager).param("examCode", examCode).get([this](const QString &jsonResponse) {
        Json json(jsonResponse.toUtf8());
        QJsonArray units = json.getJsonArray("");

        ui->unitComboBox->clear();
        ui->unitComboBox->addItem("请选择", "");

        for (QJsonArray::const_iterator iter = units.constBegin(); iter != units.constEnd(); ++iter) {
            QJsonObject unit = iter->toObject();
            ui->unitComboBox->addItem(unit.value("unit").toString(), unit.value("unit").toString());
        }
    }, [this](const QString &error) {
        showInfo(error, true);
    });
}

// 加载考点
void AiSignWidget::loadSites(const QString &examCode) {
    if (examCode.isEmpty()) { return; }

    QString url = d->serverUrl + Urls::EXAM_SITES;
    HttpClient(url).debug(d->debug).manager(d->networkManager).param("examCode", examCode).get([this](const QString &jsonResponse) {
        Json json(jsonResponse.toUtf8());
        QJsonArray sites = json.getJsonArray("");

        ui->siteComboBox->clear();
        ui->siteComboBox->addItem("请选择", "");

        for (QJsonArray::const_iterator iter = sites.constBegin(); iter != sites.constEnd(); ++iter) {
            QJsonObject site = iter->toObject();
            ui->siteComboBox->addItem(site.value("siteName").toString(), site.value("siteCode").toString());
        }

        updateSystemStatus(ui->examStatusLabel, true);
    }, [this](const QString &error) {
        showInfo(error, true);
    });
}

// 加载考点下的考场
void AiSignWidget::loadRooms(const QString &examCode, const QString &siteCode) {
    if (examCode.isEmpty() || siteCode.isEmpty()) { return; }

    QString url = d->serverUrl + Urls::EXAM_ROOMS;
    HttpClient(url).debug(d->debug).manager(d->networkManager)
            .param("examCode", examCode)
            .param("siteCode", siteCode).get([this](const QString &jsonResponse) {
        Json json(jsonResponse.toUtf8());
        QJsonArray rooms = json.getJsonArray("");

        ui->roomComboBox->clear();
        ui->roomComboBox->addItem("请选择", "");

        for (QJsonArray::const_iterator iter = rooms.constBegin(); iter != rooms.constEnd(); ++iter) {
            QJsonObject room = iter->toObject();
            ui->roomComboBox->addItem(room.value("roomCode").toString(), room.value("roomCode").toString());
        }

        ui->roomComboBox->addItem("机动", "x");
    }, [this](const QString &error) {
        showInfo(error, true);
    });

//    Site s = d->findSite(siteCode);

//    ui->roomComboBox->clear();
//    ui->roomComboBox->addItem("请选择", "");


//    foreach (const Room &room, s.rooms) {
//        ui->roomComboBox->addItem(room.roomCode, room.roomCode);
//    }

//    if (s.rooms.size() > 0) {
//        ui->roomComboBox->addItem("机动", "x");
//    }

// updateLoginStatistics(QList<Student>());
}

// 从服务器加载考试单元、考点、考场
void AiSignWidget::loadExamUnitAndSiteAndRoom(const QString &examCode) {
    // http://192.168.10.85:8080/initializeRoom
    QString url = d->serverUrl + Urls::INITIALIZE_ROOM;
    HttpClient(url).debug(d->debug).manager(d->networkManager).param("examCode", examCode).get([this](const QString &jsonResponse) {
        // [1] 解析考试单元 unit
        d->units = ResponseUtil::responseToUnits(jsonResponse);
        ui->unitComboBox->clear();
        ui->unitComboBox->addItem("请选择", "");

        foreach (const Unit &pu, d->units) {
            ui->unitComboBox->addItem(pu.unit, pu.unit);
        }

        // [2] 解析考点 Site，考点下由考场 Room
        d->sites = ResponseUtil::responseToSites(jsonResponse);
        ui->siteComboBox->clear();
        ui->siteComboBox->addItem("请选择", "");

        foreach (const Site &s, d->sites) {
            QString text = s.siteCode + "-" + s.siteName;
            ui->siteComboBox->addItem(text, s.siteCode);
            ui->siteComboBox->setItemData(ui->siteComboBox->count() - 1, text, Qt::ToolTipRole);
        }

        // showInfo("初始化完成，请选择考期、考点、考场", false);
        updateSystemStatus(ui->examStatusLabel, true);
    }, [this](const QString &error) {
        showInfo(error, true);
    });
}

// 加载学生信息
void AiSignWidget::loadStudents() {
    initializeSignInStatus(QList<Student>()); // 清空登陆统计

    QString examCode = ui->examComboBox->currentData().toString();
    QString unit     = ui->unitComboBox->currentData().toString();
    QString siteCode = ui->siteComboBox->currentData().toString();
    QString roomCode = ui->roomComboBox->currentData().toString();

    if (examCode.isEmpty() || siteCode.isEmpty() || roomCode.isEmpty() || unit.isEmpty()) {
        // showInfo("siteCode or roomCode or periodUnitCode cannot be empty", true);
        return;
    }

    // https://jk.edu-edu.com/getRoomEnrollment?examCode=tqd_xm&unit=200&siteCode=tqd_xm01&roomCode=jk_01
    QString url = d->serverUrl + Urls::GET_ROOM_ENROLLMENT;
    HttpClient(url).debug(d->debug).manager(d->networkManager)
            .param("examCode", examCode)
            .param("unit", unit)
            .param("siteCode", siteCode)
            .param("roomCode", roomCode)
            .get([this](const QString &jsonResponse) {
        Json json(jsonResponse.toUtf8());

        // [1] 错误: 为发生错误
        if (1 != json.getInt("status")) {
            showInfo(jsonResponse, true);
            return;
        }

        // [2] 显示学生信息
        d->students = ResponseUtil::responseToStudents(jsonResponse);

        initializeSignInStatus(d->students);
    }, [this](const QString &error) {
        showInfo(error, true);
    });
}

void AiSignWidget::loadServerTime() {
    // timeDiff is client current time minus server current time.
    QString url = d->serverUrl + Urls::TIMESTAMP;
    HttpClient(url).manager(d->networkManager).debug(d->debug).get([this](const QString &response) {
        d->timeDiff = QDateTime::currentMSecsSinceEpoch() - response.toLongLong();
    });
}

// 初始化学生的签到信息
void AiSignWidget::initializeSignInStatus(const QList<Student> &students) {
    int totalStudentCount = students.size();
    int unsignInStudentCount = 0; // 未登陆人数

    // 计算未签到人数
    foreach (const Student &s, students) {
        if (s.signedAt.isEmpty()) {
            unsignInStudentCount++;
        }
    }

    ui->signInStudentCountLabel->setText(QString::number(totalStudentCount - unsignInStudentCount));
    ui->unsignInStudentCountLabel->setText(QString::number(unsignInStudentCount));
    ui->totalStudentCountLabel->setText(QString::number(totalStudentCount));

    emit studentsReady(d->students);
}

// 显示信息, error 为 true 时以红色显示
void AiSignWidget::showInfo(const QString &info, bool ok) const {
    QString okImage    = "image/common/ok.png";
    QString errorImage = "image/common/error.png";
    ui->infoLabel->setProperty("ok", ok);
    ui->infoLabel->setText(QString("<img src='%1' width=16 height=16> %2")
                           .arg(ok ? okImage : errorImage).arg(info));

    UiUtil::updateQss(ui->infoLabel);
}

// 身份证信息读取成功
void AiSignWidget::personReady(const Person &p) {
    showPerson(p);

    SignInInfo info = getSignInInfo();
    if (!info.valid) { return; }

    // 保存身份证图片
    {
        QImage idCardPicture("idcard.bmp");
        idCardPicture.save(info.idCardPicturePath);
    }

    /*if (ui->signInModeSimpleButton->isChecked()) {
        // 简单签到
        d->signInMode = SignInMode::SIGN_IN_SIMPLE;
        info.signInMode = SignInMode::SIGN_IN_SIMPLE;
        signIn(info);
    } else if (ui->signInModeWithFaceButton->isChecked()) {
        // 人脸识别签到
        d->signInMode = SignInMode::SIGN_IN_WITH_FACE;
        d->cameraImageCapture->capture("capture.jpg"); // 拍照
    }*/

    if (d->signInWithFace) {
        if (ui->signInModeComboBox->currentIndex() == 1) {
            // 简单签到
            d->signInMode = SignInMode::SIGN_IN_SIMPLE;
            info.signInMode = SignInMode::SIGN_IN_SIMPLE;
            signIn(info);
        } else if (ui->signInModeComboBox->currentIndex() == 0) {
            // 人脸识别签到
            d->signInMode = SignInMode::SIGN_IN_WITH_FACE;
            d->cameraImageCapture->capture("capture.jpg"); // 拍照
        }
    } else {
        // 简单签到
        d->signInMode = SignInMode::SIGN_IN_SIMPLE;
        info.signInMode = SignInMode::SIGN_IN_SIMPLE;
        signIn(info);
    }
}

// 显示考生的身份证信息
void AiSignWidget::showPerson(const Person &p) {
    ui->nameLabel->setText(p.name);
    ui->genderLabel->setText(p.gender);
    ui->birthdayLabel->setText(Util::formatDate(p.birthday));
    ui->idCardNoLabel->setText(p.cardId);
    ui->startDateLabel->setText(Util::formatDate(p.validStart));
    ui->endDateLabel->setText(Util::formatDate(p.validEnd));
    ui->idCardPictureLabel->setPixmap(QPixmap(CardReader::personImagePath()));
}

// 获取签到的学生的信息, 如果 validateIdCard 为 true，则要先刷身份证
SignInInfo AiSignWidget::getSignInInfo(bool validateIdCard) const {
    SignInInfo info;
    info.examCode   = ui->examComboBox->currentData().toString();
    info.unit       = ui->unitComboBox->currentData().toString();
    info.siteCode   = ui->siteComboBox->currentData().toString();
    info.roomCode   = ui->roomComboBox->currentData().toString();
    info.name       = ui->nameLabel->text();
    info.idCardNo   = ui->idCardNoLabel->text();
    info.signAt     = QString::number(QDateTime::currentMSecsSinceEpoch() - d->timeDiff);
    info.idCardPicturePath = getIdCardPicturePath(info);
    info.facePicturePath = getFacePicturePath(info);
    info.writePicturePath  = getWritePicturePath(info);

    if (info.examCode.isEmpty()) {
        info.valid = false;
        showInfo("请选择考试", false);
        return info;
    }

    if (info.unit.isEmpty()) {
        info.valid = false;
        showInfo("请选择考期", false);
        return info;
    }

    if (info.siteCode.isEmpty()) {
        info.valid = false;
        showInfo("请选择考点", false);
        return info;
    }

    if (info.roomCode.isEmpty()) {
        info.valid = false;
        showInfo("请选择考场", false);
        return info;
    }

    if (validateIdCard && info.idCardNo.isEmpty()) {
        info.valid = false;
        showInfo("请先刷身份证", false);
        return info;
    }

    return info;
}

// 获取签到的学生的身份证照片路径
QString AiSignWidget::getIdCardPicturePath(const SignInInfo &info) const {
    return QString("picture-idcard/%1-%2-%3-%4.jpg")
            .arg(info.unit)
            .arg(info.siteCode)
            .arg(info.roomCode)
            .arg(info.idCardNo);
}

// 获取签到的学生的摄像头照片路径
QString AiSignWidget::getFacePicturePath(const SignInInfo &info) const {
    return QString("picture-face/%1-%2-%3-%4.jpg")
            .arg(info.unit)
            .arg(info.siteCode)
            .arg(info.roomCode)
            .arg(info.idCardNo);
}

// 获取签到的学生手写笔记照片路径
QString AiSignWidget::getWritePicturePath(const SignInInfo &info) const {
    return QString("picture-write/%1-%2-%3-%4.jpg")
            .arg(info.unit)
            .arg(info.siteCode)
            .arg(info.roomCode)
            .arg(info.idCardNo);
}

// // 更新系统状态
void AiSignWidget::updateSystemStatus(QWidget *w, bool ok) {
    w->setProperty("ok", ok);
    UiUtil::updateQss(w);
}

// 签到成功
void AiSignWidget::signInSuccess(const SignInInfo &info) const {
    showInfo(QString("%1 签到成功").arg(info.name));

    d->signInStatusWidget->signInSuccess(info);

    int signInCount = d->signInStatusWidget->signedStudentsCount();
    int unsignInCount = d->students.count() - signInCount;
    ui->signInStudentCountLabel->setText(QString::number(signInCount));
    ui->unsignInStudentCountLabel->setText(QString::number(unsignInCount));
}

// 签到
void AiSignWidget::signIn(const SignInInfo &info) const {
    if (SignInMode::SIGN_IN_SIMPLE == info.signInMode) {
        SignInService::signInSimple(d->serverUrl + Urls::SIGN_IN, info, this, d->networkManager);
    } else if (SignInMode::SIGN_IN_WITH_FACE == info.signInMode) {
        SignInService::signInWithFace(d->serverUrl + Urls::SIGN_IN_WITH_FACE, info, this, d->networkManager);
    } else if (SignInMode::SIGN_IN_WRITTING == info.signInMode) {
        // TODO: 上传手写笔迹
    }
}
