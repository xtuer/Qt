#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <functional>
#include <QHash>
#include <QUrlQuery>

class QNetworkAccessManager;

class HttpClient {
public:
    HttpClient(const QString &url);
    ~HttpClient();

    HttpClient& useManager(QNetworkAccessManager *manager);

    /**
     * @brief 增加参数
     * @param name  参数的名字
     * @param value 参数的值
     * @return 返回 HttpClient 的引用，可以用于链式调用
     */
    HttpClient& addParam(const QString &name, const QString &value);

    /**
     * @brief 增加访问头
     * @param header 访问头的名字
     * @param value  访问头的值
     * @return 返回 HttpClient 的引用，可以用于链式调用
     */
    HttpClient& addHeader(const QString &header, const QString &value);

    /**
     * @brief 执行 GET 请求
     * @param successHandler 请求成功的回调 lambda 函数
     * @param errorHandler   请求失败的回调 lambda 函数
     * @param encoding       请求响应的编码
     */
    void get(std::function<void (const QString &)> successHandler,
             std::function<void (const QString &)> errorHandler = NULL,
             const char *encoding = "UTF-8");

    /**
     * @brief 执行 POST 请求
     * @param successHandler 请求成功的回调 lambda 函数
     * @param errorHandler   请求失败的回调 lambda 函数
     * @param encoding       请求响应的编码
     */
    void post(std::function<void (const QString &)> successHandler,
             std::function<void (const QString &)> errorHandler = NULL,
             const char *encoding = "UTF-8");

private:
    /**
     * @brief 执行请求的辅助函数
     * @param posted 为 true 表示 POST 请求，为 false 表示 GET 请求
     * @param successHandler 请求成功的回调 lambda 函数
     * @param errorHandler   请求失败的回调 lambda 函数
     * @param encoding       请求响应的编码
     */
    void execute(bool posted,
                 std::function<void (const QString &)> successHandler,
                 std::function<void (const QString &)> errorHandler,
                 const char *encoding);

    QString url; // 请求的 URL
    QUrlQuery params; // 请求的参数
    QHash<QString, QString> headers; // 请求的头
    QNetworkAccessManager *manager;
    bool useInternalManager;
};

#endif // HTTPCLIENT_H
