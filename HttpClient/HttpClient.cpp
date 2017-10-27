#include "HttpClient.h"

#include <QDebug>
#include <QFile>
#include <QHash>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QHttpPart>
#include <QHttpMultiPart>

struct HttpClientPrivate {
    HttpClientPrivate(const QString &url) : url(url), networkAccessManager(NULL),
        useJson(false), useInternalNetworkAccessManager(true), debug(false) {}
    QString   url;      // 请求的 URL
    QUrlQuery params;   // 请求的参数使用 Form 格式
    QString   jsonData; // 请求的参数使用 Json 格式
    QHash<QString, QString> headers; // 请求的头
    QNetworkAccessManager *networkAccessManager;

    bool useJson; // 为 true 时 POST 请求使用 Json 格式传递参数，否则使用 Form 格式传递参数
    bool useInternalNetworkAccessManager; // 是否使用内部的 QNetworkAccessManager
    bool debug; // 为 true 时输出请求的 URL 和参数
};

// 注意: 不能在回调函数中使用 d，因为回调函数被调用时 HttpClient 对象很可能已经被释放掉了。
HttpClient::HttpClient(const QString &url) : d(new HttpClientPrivate(url)) {
    //    qDebug().noquote() << "HttpClient";
}

HttpClient::~HttpClient() {
    //    qDebug().noquote() << "~HttpClient";
    delete d;
}

HttpClient &HttpClient::useManager(QNetworkAccessManager *manager) {
    d->networkAccessManager = manager;
    d->useInternalNetworkAccessManager = false;
    return *this;
}

// 传入 debug 为 true 则使用 debug 模式，请求执行时输出请求的 URL 和参数等
HttpClient &HttpClient::debug(bool debug) {
    d->debug = debug;
    return *this;
}

// 增加参数
HttpClient &HttpClient::addParam(const QString &name, const QString &value) {
    d->params.addQueryItem(name, value);

    return *this;
}

HttpClient &HttpClient::jsonData(const QString &data) {
    d->useJson = true;
    d->jsonData = data;

    return *this;
}

// 增加访问头
HttpClient &HttpClient::addHeader(const QString &header, const QString &value) {
    d->headers[header] = value;
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

void HttpClient::download(const QString &destinationPath,
                          std::function<void ()> finishHandler,
                          std::function<void (const QString &)> errorHandler) {
    QFile *file = new QFile(destinationPath);
    bool debug = d->debug;

    if (file->open(QIODevice::WriteOnly)) {
        download([=](const QByteArray &data) {
            file->write(data);
        }, [=] {
            // 请求结束后释放文件对象.
            file->flush();
            file->close();
            file->deleteLater();

            // 不能用 d->debug，因为 d 以及被释放了
            if (debug) {
                qDebug().noquote() << QString("下载完成，保存到: %1").arg(destinationPath);
            }

            if (NULL != finishHandler) {
                finishHandler();
            }
        }, errorHandler);
    } else {
        // 打开文件出错
        if (debug) {
            qDebug().noquote() << QString("打开文件出错: %1").arg(destinationPath);
        }

        if (NULL != errorHandler) {
            errorHandler(QString("打开文件出错: %1").arg(destinationPath));
        }
    }
}

// 使用 GET 进行下载，当有数据可读取时回调 readyRead(), 大多数情况下应该在 readyRead() 里把数据保存到文件
void HttpClient::download(std::function<void (const QByteArray &)> readyRead,
                          std::function<void ()> finishHandler,
                          std::function<void (const QString &)> errorHandler) {
    if (d->debug) {
        QString params = d->params.toString();

        if (params.isEmpty()) {
            qDebug().noquote() << QString("网址: %1").arg(d->url);
        } else {
            qDebug().noquote() << QString("网址: %1?%2").arg(d->url).arg(params);
        }
    }

    // 如果是 GET 请求，并且参数不为空，则编码请求的参数，放到 URL 后面
    if (!d->params.isEmpty()) {
        d->url += "?" + d->params.toString(QUrl::FullyEncoded);
    }

    QUrl urlx(d->url);
    QNetworkRequest request(urlx);
    bool internal = d->useInternalNetworkAccessManager;
    QNetworkAccessManager *manager = internal ? new QNetworkAccessManager() : d->networkAccessManager;
    QNetworkReply *reply = manager->get(request);

    // 有数据可读取时回调 readyRead()
    QObject::connect(reply, &QNetworkReply::readyRead, [=] {
        readyRead(reply->readAll());
    });

    // 请求结束
    QObject::connect(reply, &QNetworkReply::finished, [=] {
        if (reply->error() == QNetworkReply::NoError && NULL != finishHandler) {
            finishHandler();
        }

        // 释放资源
        reply->deleteLater();
        if (internal) {
            manager->deleteLater();
        }
    });

    // 请求错误处理
    QObject::connect(reply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), [=] {
        if (NULL != errorHandler) {
            errorHandler(reply->errorString());
        }
    });
}

void HttpClient::upload(const QString &path,
                        std::function<void (const QString &)> successHandler,
                        std::function<void (const QString &)> errorHandler,
                        const char *encoding) {
    if (d->debug) {
        qDebug().noquote() << QString("URL: %1").arg(d->url);
    }

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QFile *file = new QFile(path);
    file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart

    // 如果文件打开失败，则释放资源返回
    if(!file->open(QIODevice::ReadOnly)) {
        if (NULL != errorHandler) {
            errorHandler(QString("文件打开失败: %1").arg(file->errorString()));
            multiPart->deleteLater();
            return;
        }
    }

    // 表明是文件上传
    QString disposition = QString("form-data; name=\"file\"; filename=\"%1\"").arg(file->fileName());
    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(disposition));
    imagePart.setBodyDevice(file);
    multiPart->append(imagePart);

    bool internal = d->useInternalNetworkAccessManager;
    QNetworkRequest request(QUrl(d->url));
    QNetworkAccessManager *manager = internal ? new QNetworkAccessManager() : d->networkAccessManager;
    QNetworkReply *reply = manager->post(request, multiPart);
    multiPart->setParent(reply);

    // 请求结束时一次性读取所有响应数据
    QObject::connect(reply, &QNetworkReply::finished, [=] {
        if (reply->error() == QNetworkReply::NoError && NULL != successHandler) {
            successHandler(readResponse(reply, encoding)); // 成功执行
        }

        // 释放资源
        reply->deleteLater();
        if (internal) {
            manager->deleteLater();
        }
    });

    // 请求错误处理
    QObject::connect(reply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), [=] {
        if (NULL != errorHandler) {
            errorHandler(reply->errorString());
        }
    });
}

// 执行请求的辅助函数
void HttpClient::execute(bool posted,
                         std::function<void (const QString &)> successHandler,
                         std::function<void (const QString &)> errorHandler,
                         const char *encoding) {
    // 如果是 GET 请求，并且参数不为空，则编码请求的参数，放到 URL 后面
    if (!posted && !d->params.isEmpty()) {
        d->url += "?" + d->params.toString(QUrl::FullyEncoded);
    }

    // 输出调试信息
    if (d->debug) {
        qDebug().noquote() << "网址:" << d->url;

        if (posted && d->useJson) {
            qDebug().noquote() << "参数:" << d->jsonData;
        } else if (posted && !d->useJson) {
            qDebug().noquote() << "参数:" << d->params.toString();
        }
    }

    QUrl urlx(d->url);
    QNetworkRequest request(urlx);

    // 如果是 POST 请求，useJson 为 true 时添加 Json 的请求头，useJson 为 false 时添加 Form 的请求头
    if (posted && !d->useJson) {
        addHeader("Content-Type", "application/x-www-form-urlencoded");
    } else if (posted && d->useJson) {
        addHeader("Accept", "application/json; charset=utf-8");
        addHeader("Content-Type", "application/json");
    }

    // 把请求的头添加到 request 中
    QHashIterator<QString, QString> iter(d->headers);
    while (iter.hasNext()) {
        iter.next();
        request.setRawHeader(iter.key().toUtf8(), iter.value().toUtf8());
    }

    // 注意: 不能在 Lambda 表达式里使用 HttpClient 对象的成员数据，因其可能在网络访问未结束时就已经被析构掉了，
    // 所以如果要使用它的相关数据，定义一个局部变量来保存其数据，然后在 Lambda 表达式里访问这个局部变量

    // 如果不使用外部的 manager 则创建一个新的，在访问完成后会自动删除掉
    bool internal = d->useInternalNetworkAccessManager;
    QNetworkAccessManager *manager = internal ? new QNetworkAccessManager() : d->networkAccessManager;
    QNetworkReply *reply = NULL;

    if (!posted) {
        // GET 请求
        reply = manager->get(request);
    } else if (posted && d->useJson) {
        // POST 请求，参数使用 Json 格式
        reply = manager->post(request, d->jsonData.toUtf8());
    } else if (posted && !d->useJson) {
        // POST 请求，参数使用 Form 格式
        reply = manager->post(request, d->params.toString(QUrl::FullyEncoded).toUtf8());
    }

    // 请求结束时一次性读取所有响应数据
    QObject::connect(reply, &QNetworkReply::finished, [=] {
        if (reply->error() == QNetworkReply::NoError && NULL != successHandler) {
            successHandler(readResponse(reply, encoding)); // 成功执行
        }

        // 释放资源
        reply->deleteLater();
        if (internal) {
            manager->deleteLater();
        }
    });

    // 请求错误处理
    QObject::connect(reply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), [=] {
        if (NULL != errorHandler) {
            errorHandler(reply->errorString());
        }
    });
}

QString HttpClient::readResponse(QNetworkReply *reply, const char *encoding) {
    QTextStream in(reply);
    QString result;
    in.setCodec(encoding);

    while (!in.atEnd()) {
        result += in.readLine();
    }

    return result;
}
