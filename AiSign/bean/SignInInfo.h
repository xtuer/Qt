#ifndef SIGNININFO_H
#define SIGNININFO_H

#include <QString>

/**
 * 签到信息
 */
class SignInInfo {
public:
    SignInInfo();

    QString periodCode; // 考期
    QString siteCode;   // 考点
    QString roomCode;   // 考场
    QString idCardNo;   // 身份证号
    QString name;       // 名字
    QString idCardPicturePath; // 身份证照片的路径
    QString facePicturePath;   // 人脸的照片的路径
    QString writePicturePath;  // 手写笔迹照片路径
    QString signAt;            // 签到时间

    bool valid = true; // 签到信息是否有效
};

#endif // SIGNININFO_H
