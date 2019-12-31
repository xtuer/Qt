#ifndef CONSTANTS_H
#define CONSTANTS_H

class QString;

namespace Constants {
    extern const int     CODE_READ_READY;
    extern const QString INFO_READ_READY;
}

namespace Urls {
    extern const QString EXAM;
    extern const QString EXAM_UNITS;
    extern const QString EXAM_SITES;
    extern const QString EXAM_ROOMS;
    extern const QString SIGN_IN;
    extern const QString SIGN_IN_WITH_FACE;
    extern const QString SIGN_IN_MANUAL;
    extern const QString INITIALIZE_ROOM;
    extern const QString GET_ROOM_ENROLLMENT;
    extern const QString TIMESTAMP;
    extern const QString UPLOAD_PHOTO;
    extern const QString CLOSE_PASSWORD;
}

// 签到模式
enum SignInMode {
    SIGN_IN_SIMPLE,    // 普通签到模式: 上传身份证照片
    SIGN_IN_WITH_FACE, // 人脸识别签到: 上传身份证照片和摄像头照片
    SIGN_IN_MANUALLY,  // 人工签到模式: 不上传照片
    SIGN_IN_WRITTING   // 上传手写签名: 上传摄像头照片
};

#endif // CONSTANTS_H
