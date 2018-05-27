#ifndef SIGNINSERVICE_H
#define SIGNINSERVICE_H

class QString;
class SignInInfo;
class AiSignWidget;
class QNetworkAccessManager;

class SignInService {
public:
    /**
     * 普通签到
     *
     * @param url  签到的 URL
     * @param info 签到的信息
     * @param asw  AiSignWidget
     * @param networkManager
     */
    static void signIn(const QString &url,
                       const SignInInfo &info,
                       const AiSignWidget *asw,
                       QNetworkAccessManager *networkManager);

    /**
     * 人脸识别签到
     *
     * @param url  签到的 URL
     * @param info 签到的信息
     * @param asw  AiSignWidget
     * @param networkManager
     */
    static void signInWithFace(const QString &url,
                               const SignInInfo &info,
                               const AiSignWidget *asw,
                               QNetworkAccessManager *networkManager);

    /**
     * 人工签到
     *
     * @param url  签到的 URL
     * @param info 签到的信息
     * @param asw  AiSignWidget
     * @param networkManager
     */
    static void signInManually(const QString &url,
                               const SignInInfo &info,
                               const AiSignWidget *asw,
                               QNetworkAccessManager *networkManager);
};

#endif // SIGNINSERVICE_H
