#ifndef NETWORKUTIL_H
#define NETWORKUTIL_H

#include <functional>
#include <QHash>
#include <QUrlQuery>

class QString;
class QNetworkAccessManager;

class NetworkUtil {
public:
    NetworkUtil();
    NetworkUtil& addParam(const QString &name, const QString &value);

private:
    QUrlQuery postData;
    QNetworkAccessManager *manager;
};

#endif // NETWORKUTIL_H
