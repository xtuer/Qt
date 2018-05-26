#include "ui_AiSignWidget.h"
#include "AiSignWidget.h"
#include "InputDialog.h"

#include "Constants.h"

#include "bean/Period.h"
#include "bean/Site.h"
#include "bean/Room.h"
#include "bean/Student.h"

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

/*-----------------------------------------------------------------------------|
 |                             AiSignWidgetPrivate                             |
 |----------------------------------------------------------------------------*/
struct AiSignWidgetPrivate {
    AiSignWidgetPrivate(AiSignWidget *aiSignWidget);
    ~AiSignWidgetPrivate();

    void initializeCamera(AiSignWidget *aiSignWidget); // 初始化摄像头
    Site findSite(const QString &siteCode) const; // 使用 siteCode 从 sites 里查找第一个有相同 siteCode 的 site

    // 摄像头
    QCamera *camera;
    QCameraViewfinder *cameraViewfinder;
    QCameraImageCapture *cameraImageCapture;

    // 考试信息: 考期、考点、考场、学生
    QList<Period>  periods;  // 考期
    QList<Site>    sites;    // 考点、考点的成员变量里有考场
    QList<Student> students; // 学生

    bool    signInWithFace; // 使用人脸识别签名
    QString serverUrl;      // 服务器的 URL
    qint64  timeDiff;       // 客户端和服务器的时间差，单位为毫秒
    QNetworkAccessManager *networkManager;
};

AiSignWidgetPrivate::AiSignWidgetPrivate(AiSignWidget *aiSignWidget) {
    initializeCamera(aiSignWidget); // 初始化摄像头

    serverUrl = ConfigInstance.getServerUrl();
    signInWithFace = ConfigInstance.isSignInWithFace();
    networkManager = new QNetworkAccessManager();
}

AiSignWidgetPrivate::~AiSignWidgetPrivate() {
    camera->stop();
    delete networkManager;
}

// 初始化摄像头
void AiSignWidgetPrivate::initializeCamera(AiSignWidget *aiSignWidget) {
    // 1. 创建设置摄像头相关的对象
    // 2. 启动摄像头
    // 3. 增加定位人脸在摄像头图像的 label
    QGridLayout *layout = qobject_cast<QGridLayout *>(aiSignWidget->ui->cameraContainer->layout());

    // [1] 创建设置摄像头相关的对象
    // [2] 启动摄像头
    camera = new QCamera(aiSignWidget);
    cameraViewfinder = new QCameraViewfinder(aiSignWidget);
    cameraImageCapture = new QCameraImageCapture(camera);
    cameraViewfinder->setProperty("class", "CameraWidget");
    layout->replaceWidget(aiSignWidget->ui->cameraPlaceholderLabel, cameraViewfinder);
    aiSignWidget->ui->cameraPlaceholderLabel->hide();
    camera->setViewfinder(cameraViewfinder);
//    camera->start();

    // [3] 定位人脸在摄像头图像的 label
    QLabel *facePositionLabel = new QLabel();
    facePositionLabel->setObjectName("facePositionLabel");
    layout->addWidget(facePositionLabel, 0, 0);
    layout->setAlignment(facePositionLabel, Qt::AlignCenter);
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
    delete ui;
    delete d;
}

// 初始化
void AiSignWidget::initialize() {
    setAttribute(Qt::WA_StyledBackground);
    ui->cameraContainer->layout()->setAlignment(ui->captureAndUploadButton, Qt::AlignHCenter); // 按钮居中

    d = new AiSignWidgetPrivate(this);
    loadPeriodUnitAndSiteAndRoom();
    loadServerTime();
}

// 信号槽事件处理
void AiSignWidget::handleEvents() {
    // [Camera] 点击拍照上传按钮进行拍照
    connect(ui->captureAndUploadButton, &QPushButton::clicked, [this] {
        d->cameraImageCapture->capture("capture.jpg"); // 如果不传入截图文件的路径，则会使用默认的路径，每次截图生成一个图片
    });

    // [Camera] 拍照完成后的槽函数，可以把 image 保存到自己想要的位置
    QObject::connect(d->cameraImageCapture, &QCameraImageCapture::imageCaptured, [this](int id, const QImage &image) {
        Q_UNUSED(id)
        // 1. 显示预览图
        // 1.1 预览图显示到右边
        // 1.2 预览图显示到中间，和身份证照片进行比较: 按高度缩放，然后截取中间部分
        // 2. 保存到系统
        // 3. 签到

        // [1.1] 预览图显示到右边
        QImage rightImage = image.scaled(ui->previewLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->previewLabel->setPixmap(QPixmap::fromImage(rightImage));

        // [1.2] 预览图显示到中间，和身份证照片进行比较: 按高度缩放，然后截取中间部分
        QImage centerImage = image.scaledToHeight(ui->cameraPictureLabel->size().height(), Qt::SmoothTransformation);
        int x = (centerImage.width() - ui->cameraPictureLabel->width()) / 2;
        int w = ui->cameraPictureLabel->width();
        int h = ui->cameraPictureLabel->height();
        ui->cameraPictureLabel->setPixmap(QPixmap::fromImage(centerImage.copy(x, 0, w, h)));
    });

    // [考试信息] 当 Period 变化时，取消 Room 的选择
    connect(ui->periodComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [this]() {
        ui->roomComboBox->setCurrentIndex(0);
    });

    // [考试信息] 当 Site 变化时，重行初始化 Room 信息
    connect(ui->siteComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index) {
        QString siteCode = ui->siteComboBox->itemData(index).toString();
        loadRoom(siteCode);
    });

    // [考试信息] 当 Room 变化时，加载学生的签到信息
    connect(ui->roomComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [this]() {
        loadStudents();
    });

    // [签到] 手动签到
    connect(ui->signInManualButton, &QPushButton::clicked, [this] {
        signInManually();
    });
}

// 从服务器加载考期、考点、考场
void AiSignWidget::loadPeriodUnitAndSiteAndRoom() {
    // http://192.168.10.85:8080/initializeRoom
    QString url = d->serverUrl + Urls::INITIALIZE_ROOM;
    HttpClient(url).debug(true).manager(d->networkManager).get([this](const QString &jsonResponse) {
        // 解析考期 Period
        d->periods = ResponseUtil::responseToPeroids(jsonResponse);
        ui->periodComboBox->clear();
        ui->periodComboBox->addItem("请选择", "");

        foreach (const Period &pu, d->periods) {
            ui->periodComboBox->addItem(pu.period + "-" + pu.unit + " 单元", pu.periodCode);
        }

        // 解析考点 Site
        d->sites = ResponseUtil::responseToSites(jsonResponse);
        ui->siteComboBox->clear();
        ui->siteComboBox->addItem("请选择", "");

        foreach (const Site &s, d->sites) {
            QString text = s.siteCode + "-" + s.siteName;
            ui->siteComboBox->addItem(text, s.siteCode);
            ui->siteComboBox->setItemData(ui->siteComboBox->count() - 1, text, Qt::ToolTipRole);
        }

        showInfo("初始化完成，请选择考期、考点、考场", false);
    }, [this](const QString &error) {
        showInfo(error, true);
    });
}

// 加载考点下的考场
void AiSignWidget::loadRoom(const QString &siteCode) {
    Site s = d->findSite(siteCode);

    ui->roomComboBox->clear();
    ui->roomComboBox->addItem("请选择", "");
//    updateLoginStatistics(QList<Student>());

    foreach (const Room &room, s.rooms) {
        ui->roomComboBox->addItem(room.roomCode, room.roomCode);
    }

    if (s.rooms.size() > 0) {
        ui->roomComboBox->addItem("机动", "x");
    }
}

// 加载学生信息
void AiSignWidget::loadStudents() {
    updateSignInStatus(QList<Student>()); // 清空登陆统计

    QString siteCode = ui->siteComboBox->currentData().toString();
    QString roomCode = ui->roomComboBox->currentData().toString();
    QString periodCode = ui->periodComboBox->currentData().toString();

    if (siteCode.isEmpty() || roomCode.isEmpty() || periodCode.isEmpty()) {
        // showInfo("siteCode or roomCode or periodUnitCode cannot be empty", true);
        return;
    }

    // http://192.168.10.95:8080/getRoomEnrollment?siteCode=0105013&roomCode=11&periodUnitCode=16090001
    QString url = d->serverUrl + Urls::GET_ROOM_ENROLLMENT;
    HttpClient(url).debug(true).manager(d->networkManager).param("siteCode", siteCode)
            .param("roomCode", roomCode).param("periodUnitCode", periodCode)
            .get([this](const QString &jsonResponse) {
        Json json(jsonResponse.toUtf8());

        // 1 为发生错误
        if (1 != json.getInt("status")) {
            showInfo(jsonResponse, true);
            return;
        }

        // 显示学生信息
        d->students = ResponseUtil::responseToStudents(jsonResponse);
        updateSignInStatus(d->students);
    }, [this](const QString &error) {
        showInfo(error, true);
    });
}

void AiSignWidget::loadServerTime() {
    // timeDiff is client current time minus server current time.
    QString url = d->serverUrl + Urls::TIMESTAMP;
    HttpClient(url).manager(d->networkManager).debug(true).get([this](const QString &response) {
        d->timeDiff = QDateTime::currentMSecsSinceEpoch() - response.toLongLong();
    });
}

// 人工签到
void AiSignWidget::signInManually() {
    QString siteCode = ui->siteComboBox->currentData().toString();
    QString roomCode = ui->roomComboBox->currentData().toString();
    QString periodUnitCode = ui->periodComboBox->currentData().toString();

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

    InputDialog dlg(this);
    dlg.setWindowFlags(dlg.windowFlags() ^ Qt::WindowContextHelpButtonHint);
    if (QDialog::Accepted != dlg.exec()) {
        return;
    }

    QString examineeName = dlg.getExamineeName();
    QString idCardNo     = dlg.getIdCardNo();

    QString signAt = QString::number(QDateTime::currentMSecsSinceEpoch() - d->timeDiff);
    QString sign   = Util::md5(QString("%1%2").arg(idCardNo).arg(signAt).toUtf8());
    QString url    = d->serverUrl + Urls::SIGN_IN_MANUAL;
    HttpClient(url).debug(true).manager(d->networkManager)
            .param("idCardNo", idCardNo)
            .param("examineeName", examineeName)
            .param("siteCode", siteCode)
            .param("roomCode", roomCode)
            .param("periodUnitCode", periodUnitCode)
            .param("signAt", signAt)
            .param("sign", sign)
            .post([=](const QString &response) {
        Json json(response.toUtf8());

        if (1 != json.getInt("statusCode")) {
            showInfo(QString("%1 %2").arg(examineeName).arg(json.getString("message")), true);
            return;
        }

        showInfo(QString("%1 签到成功").arg(examineeName));
//        loginSuccess(idCardNo);
    }, [this](const QString &error) {
        showInfo(error, true);
    });
}

// 更新学生的签到信息
void AiSignWidget::updateSignInStatus(const QList<Student> &students) {
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
}

void AiSignWidget::showInfo(const QString &info, bool error) {
    ui->infoLabel->setProperty("error", error);
    ui->infoLabel->setText(info);

    UiUtil::updateQss(ui->infoLabel);
}
