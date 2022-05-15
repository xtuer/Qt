#include "Widget.h"

#include <QApplication>
#include <QDateTime>
#include <QMap>

#include "HttpClient.h"
#include "Json.h"

/**
 * 生成签名，登录使用
 */
QString sign(const QString appId, const QString appKey, const QString username, const QString password, qint64 timestamp) {
    /*
     签名计算逻辑:
     1. 把参数按照参数名字母序排列 (使用 TreeMap)
     2. 参数和参数值使用等号 = 连接，参数对之间使用 & 连接
     3. 对参数拼接得到的字符串求 MD5 即签名
     */
    QString params = QString("appId=%1&appKey=%2&password=%3&timestamp=%4&username=%5")
            .arg(appId)
            .arg(appKey)
            .arg(password)
            .arg(timestamp)
            .arg(username);

    QString md5 = QCryptographicHash::hash(params.toUtf8(), QCryptographicHash::Md5).toHex();
    return md5;
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // 登录地址，不要修改
    QString url("http://118.195.181.149:8081/api/login/oauth");

    QString username = "alice";  // TODO: 账号，用户输入
    QString password = "123456"; // TODO: 密码，用户输入

    QString appId  = "boobytech.com"; // 应用的 ID，不要修改
    QString appKey = "P@ssw0rd";      // 应用的 Key，不要修改
    qint64 timestamp = QDateTime::currentMSecsSinceEpoch();
    QString loginSign = sign(appId, appKey, username, password, timestamp);

    // 修改 LoginWidget::handleEvents 的 ui->loginButton 点击事件，在事件中调用下面的代码进行登录
    HttpClient(url).debug(true).header("X-Requested-With", "XMLHttpRequest")
            .param("orgId", 1)
            .param("appId", appId)
            .param("username", username)
            .param("password", password)
            .param("timestamp", timestamp)
            .param("sign", loginSign)
            .success([](const QString &response) {
                Json json(response);
                if (json.getBool("data")) {
                    qDebug() << "登录成功";
                    // 登录成功后调用 UiUtil::topLevelWidget(this)->close(); 关闭登录窗口
                } else {
                    qDebug() << "登录失败";
                }
            }).post();

    return a.exec();
}
