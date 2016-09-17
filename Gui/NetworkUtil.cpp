#include "NetworkUtil.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QTextStream>


NetworkUtil::NetworkUtil() {

}

NetworkUtil& NetworkUtil::addParam(const QString &name, const QString &value) {
    postData.addQueryItem(name, value);
    return *this;
}
