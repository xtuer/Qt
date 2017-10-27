#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <functional>

class QString;
class QByteArray;
struct HttpClientPrivate;
class QNetworkReply;
class QNetworkAccessManager;

/**
 * @brief 对 QNetworkAccessManager 进行封装的 HTTP 访问客户端，可以进行 GET，POST，上传，下载请求。
 */
class HttpClient {
public:
    HttpClient(const QString &url);
    ~HttpClient();

    /**
     * @brief 每创建一个 QNetworkAccessManager 对象都会创建一个线程，当频繁的访问网络时，为了节省线程资源，
     *     可以传入 QNetworkAccessManager 给多个请求共享(它不会被 HttpClient 删除，用户需要自己手动删除)。
     *     如果没有使用 useManager() 传入一个 QNetworkAccessManager，则 HttpClient 会自动的创建一个，并且在网络访问完成后删除它。
     * @param  manager QNetworkAccessManager 对象
     * @return 返回 HttpClient 的引用，可以用于链式调用
     */
    HttpClient& useManager(QNetworkAccessManager *manager);

    /**
     * @brief  参数 debug 为 true 则使用 debug 模式，请求执行时输出请求的 URL 和参数等
     * @param  debug 是否启用调试模式
     * @return 返回 HttpClient 的引用，可以用于链式调用
     */
    HttpClient& debug(bool debug);

    /**
     * @brief 添加请求的参数
     * @param name  参数的名字
     * @param value 参数的值
     * @return 返回 HttpClient 的引用，可以用于链式调用
     */
    HttpClient& addParam(const QString &name, const QString &value);

    /**
     * @brief 添加请求的参数，使用 Json 格式，例如 "{\"name\": \"Alice\"}"
     * @param data Json 格式的参数字符串
     * @return
     */
    HttpClient& jsonData(const QString &data);

    /**
     * @brief 添加请求头
     * @param header 请求头的名字
     * @param value  请求头的值
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

    /**
     * @brief 使用 GET 进行下载，下载的文件保存到 destinationPath
     * @param destinationPath 下载的文件保存路径
     * @param finishHandler   请求处理完成后的回调 lambda 函数
     * @param errorHandler    请求失败的回调 lambda 函数，打开文件 destinationPath 出错也会调用此函数
     */
    void download(const QString &destinationPath,
                  std::function<void ()> finishHandler = NULL,
                  std::function<void (const QString &)> errorHandler = NULL);

    /**
     * @brief 使用 GET 进行下载，当有数据可读取时回调 readyRead(), 大多数情况下应该在 readyRead() 里把数据保存到文件
     * @param readyRead     有数据可读取时的回调 lambda 函数
     * @param finishHandler 请求处理完成后的回调 lambda 函数
     * @param errorHandler  请求失败的回调 lambda 函数
     */
    void download(std::function<void (const QByteArray &)> readyRead,
                  std::function<void ()> finishHandler = NULL,
                  std::function<void (const QString &)> errorHandler = NULL);

    /**
     * @brief 上传文件
     * @param path 要上传的文件的路径
     * @param successHandler 请求成功的回调 lambda 函数
     * @param errorHandler   请求失败的回调 lambda 函数
     * @param encoding       请求响应的编码
     */
    void upload(const QString &path, std::function<void (const QString &)> successHandler = NULL,
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

    /**
     * @brief 读取服务器响应的数据
     * @param reply 请求的 QNetworkReply 对象
     * @param encoding 请求响应的编码，默认使用 UTF-8
     * @return 服务器端响应的字符串
     */
    QString readResponse(QNetworkReply *reply, const char *encoding = "UTF-8");

    HttpClientPrivate *d;
};

#endif // HTTPCLIENT_H
