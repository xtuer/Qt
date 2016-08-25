#include "CommonUtil.h"

#include "util/DBUtil.h"
#include "util/SqlUtil.h"
#include "util/SettingUtil.h"

// 设置 Debug 输出的格式
void CommonUtil::registerMessagePattern() {
    QString QT_MESSAGE_PATTERN("[%{if-debug}D%{endif}]: %{file}:%{line} - %{function}() - %{message}");
    qSetMessagePattern(QT_MESSAGE_PATTERN);
}

// 清理释放资源
void CommonUtil::releaseSingletonResource() {
    SqlUtil::release();
    SettingUtil::release();
    DBUtil::removeDatabase();
}
