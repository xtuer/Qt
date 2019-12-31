#include "Constants.h"

#include <QString>

namespace Constants {
    const int     CODE_READ_READY = 10000;
    const QString INFO_READ_READY = "开始刷卡";
}

namespace Urls {
    const QString EXAM              = "/getExamForSignIn";   // 考试
    const QString EXAM_UNITS        = "/findUnitsForSignIn"; // 考试单元
    const QString EXAM_SITES        = "/getSitesForSignIn";  // 考点
    const QString EXAM_ROOMS        = "/findRoomsForSignIn"; // 考场
    const QString ROOM_ENROLLMENTS  = "/getRoomEnrollment";  // 考生

    const QString SIGN_IN           = "/signIn";
    const QString SIGN_IN_WITH_FACE = "/signInWithFace";
    const QString SIGN_IN_MANUAL    = "/invigilatorSignIn";
    const QString INITIALIZE_ROOM   = "/initializeRoom";
    const QString TIMESTAMP         = "/timestamp";
    const QString UPLOAD_PHOTO      = "/uploadPhoto";
    const QString CLOSE_PASSWORD    = "/getClientClosePassword"; // 申请客户端关机密码
}
