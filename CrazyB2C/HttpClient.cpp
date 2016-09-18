#include "HttpClient.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QDebug>

HttpClient::HttpClient(const QString &url) : url(url), manager(NULL), useInternalManager(true) {
//    qDebug() << "HttpClient";
}

HttpClient::~HttpClient() {
//    qDebug() << "~HttpClient";
}

HttpClient &HttpClient::useManager(QNetworkAccessManager *manager) {
    this->manager = manager;
    this->useInternalManager = false;
    return *this;
}

// 增加参数
HttpClient &HttpClient::addParam(const QString &name, const QString &value) {
    params.addQueryItem(name, value);
    return *this;
}

// 增加访问头
HttpClient &HttpClient::addHeader(const QString &header, const QString &value) {
    headers[header] = value;
    return *this;
}

// 执行 GET 请求
void HttpClient::get(std::function<void (const QString &)> successHandler,
                     std::function<void (const QString &)> errorHandler,
                     const char *encoding) {
    execute(false, successHandler, errorHandler, encoding);
}

// 执行 POST 请求
void HttpClient::post(std::function<void (const QString &)> successHandler,
                      std::function<void (const QString &)> errorHandler,
                      const char *encoding) {
    execute(true, successHandler, errorHandler, encoding);
}

// 执行请求的辅助函数
void HttpClient::execute(bool posted,
                         std::function<void (const QString &)> successHandler,
                         std::function<void (const QString &)> errorHandler,
                         const char *encoding) {
    // 如果是 GET 请求，并且参数不为空，则编码请求的参数，放到 URL 后面
    if (!posted && !params.isEmpty()) {
        url += "?" + params.toString(QUrl::FullyEncoded);
    }

    QUrl urlx(url);
    QNetworkRequest request(urlx);

    // 把请求的头添加到 request 中
    QHashIterator<QString, QString> iter(headers);
    while (iter.hasNext()) {
        iter.next();
        request.setRawHeader(iter.key().toUtf8(), iter.value().toUtf8());
    }

    QNetworkAccessManager *manager = useInternalManager ? new QNetworkAccessManager() : this->manager;
    QNetworkReply *reply = posted ? manager->post(request, params.toString(QUrl::FullyEncoded).toUtf8()) : manager->get(request);

    // 请求错误处理
    QObject::connect(reply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), [=] {
        if (NULL != errorHandler) {
            errorHandler(reply->errorString());
        }
    });

    // 请求结束时一次性读取所有响应数据
    QObject::connect(reply, &QNetworkReply::finished, [=] {
        if (reply->error() == QNetworkReply::NoError && NULL != successHandler) {
            // 读取响应数据
            QTextStream in(reply);
            QString result;
            in.setCodec(encoding);

            while (!in.atEnd()) {
                result += in.readLine();
            }

            successHandler(result);
        }

        // 释放资源
        reply->deleteLater();
        if (useInternalManager) {
            manager->deleteLater();
        }
    });
}
