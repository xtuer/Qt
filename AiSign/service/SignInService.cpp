#include "SignInService.h"
#include "bean/SignInInfo.h"
#include "gui/AiSignWidget.h"
#include "util/Util.h"
#include "util/Json.h"
#include "util/HttpClient.h"

#include <QDebug>
#include <QStringList>

// 普通签到
void SignInService::signInSimple(const QString &url,
                                 const SignInInfo &info,
                                 const AiSignWidget *asw,
                                 QNetworkAccessManager *networkManager) {
    QString sign = Util::md5(QString("%1%2").arg(info.idCardNo).arg(info.signAt).toUtf8());

    HttpClient(url).debug(true).manager(networkManager)
            .param("idCardNo", info.idCardNo)
            .param("examineeName", info.name)
            .param("examCode", info.examCode)
            .param("unit", info.unit)
            .param("siteCode", info.siteCode)
            .param("roomCode", info.roomCode)
            .param("signAt", info.signAt)
            .param("sign", sign)
            .upload(info.idCardPicturePath, [=](const QString &response) {
        SignInService::successHandler(info, response, asw);
    }, [=](const QString &error) {
        SignInService::failHandler(info, error, asw);
    });
}

// 人脸识别签到
void SignInService::signInWithFace(const QString &url,
                                   const SignInInfo &info,
                                   const AiSignWidget *asw,
                                   QNetworkAccessManager *networkManager) {
    QStringList pictures = QStringList() << info.idCardPicturePath << info.facePicturePath;
    QString sign = Util::md5(QString("%1%2").arg(info.idCardNo).arg(info.signAt).toUtf8());

    HttpClient(url).debug(true).manager(networkManager)
            .param("idCardNo", info.idCardNo)
            .param("examineeName", info.name)
            .param("examCode", info.examCode)
            .param("unit", info.unit)
            .param("siteCode", info.siteCode)
            .param("roomCode", info.roomCode)
            .param("signAt", info.signAt)
            .param("sign", sign)
            .upload(pictures, [=](const QString &response) {
        SignInService::successHandler(info, response, asw);
    }, [=](const QString &error) {
        SignInService::failHandler(info, error, asw);
    });
}

// 人工签到
void SignInService::signInManually(const QString &url,
                                   const SignInInfo &info,
                                   const AiSignWidget *asw,
                                   QNetworkAccessManager *networkManager) {
    QString sign = Util::md5(QString("%1%2").arg(info.examUid).arg(info.signAt).toUtf8());

    HttpClient(url).debug(true).manager(networkManager)
            .param("examUId", info.examUid)
            .param("examineeName", info.name)
            .param("examCode", info.examCode)
            .param("unit", info.unit)
            .param("siteCode", info.siteCode)
            .param("roomCode", info.roomCode)
            .param("password", info.password)
            .param("signAt", info.signAt)
            .param("sign", sign)
            .post([=](const QString &response) {
        SignInService::successHandler(info, response, asw);
    }, [=](const QString &error) {
        SignInService::failHandler(info, error, asw);
    });
}

void SignInService::successHandler(const SignInInfo &info, const QString &response, const AiSignWidget *asw) {
    Json json(response.toUtf8());

    if (!json.isValid()) {
        asw->showInfo(response, false);
        return;
    }

    // statusCode 为 1 表示成功
    if (1 != json.getInt("statusCode")) {
        asw->showInfo(QString("%1 %2").arg(info.name).arg(json.getString("message")), false);
    } else {
        asw->signInSuccess(info);
    }
}

void SignInService::failHandler(const SignInInfo &info, const QString &error, const AiSignWidget *asw) {
    asw->showInfo(error, false);
}
