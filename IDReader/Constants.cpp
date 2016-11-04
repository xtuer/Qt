#include "Constants.h"

#include <QString>

namespace Constants {
    const int CODE_READ_READY = 10000;
    const QString INFO_READ_READY = "连接身份证阅读器成功，请放入身份证进行刷卡";
}

namespace Urls {
    const QString SIGN_IN = "/signIn";
    const QString INITIALIZE_ROOM = "/initializeRoom";
    const QString GET_ROOM_ENROLLMENT = "/getRoomEnrollment";
}
