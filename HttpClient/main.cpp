#include "HttpClient.h"

#include <QDebug>
#include <QApplication>
#include <QNetworkAccessManager>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // 在代码块里执行网络访问，是为了测试 HttpClient 对象在被析构后，网络访问的回调函数仍然能正常执行
    {
        QString url("http://localhost:8080/api/rest");

        // [1] GET 请求无参数
        HttpClient(url).success([](const QString &response) {
            qDebug().noquote() << response;
        }).get();

        // [2] GET 请求有参数，有自定义 header，有失败的回调函数
        // 提示: 多个参数也可以传入 map: HttpClient(url).params({{"name", "诸葛亮"}, {"attackDamage", "99"}}).get(...);
        HttpClient(url).debug(true).param("name", "诸葛亮").param("value", 99).header("token", "md5sum").success([](const QString &response) {
            qDebug().noquote() << response;
        }).fail([](const QString &error, int errorCode) {
            qDebug().noquote() << error << errorCode;
        }).get();

        // [3] POST 请求，使用 param 添加参数，请求的参数使用 Form 格式
        HttpClient(url).debug(true).param("name", "卧龙").param("value", 99).success([](const QString &response) {
            qDebug().noquote() << response;
        }).post();

        // [4] PUT 请求，使用 json 添加参数，请求的参数使用 Json 格式
        HttpClient(url).debug(true).json("{\"name\": \"孔明\"}").success([](const QString &response) {
            qDebug().noquote() << response;
        }).put();

        // [5] DELETE 请求
        HttpClient(url).debug(true).success([](const QString &response) {
            qDebug().noquote() << response;
        }).remove();
    }

    {
        // [6] 下载: 保存到文件
        HttpClient("http://qtdebug.com/img/dog.png").debug(true).success([](const QString &response) {
            qDebug().noquote() << response;
        }).download("/Users/Biao/Desktop/dog-1.png");
    }

    {
        // 上传的同时能够传递参数
        // [7] 上传一个文件
        HttpClient("http://localhost:8080/api/upload").debug(true).upload(QString("/Users/Biao/Pictures/ade.jpg"));

        // [8] 上传多个文件
        HttpClient("http://localhost:8080/api/uploads").debug(true).param("name", "Biao").success([](const QString &response) {
            qDebug().noquote() << response;
        }).upload({ "/Users/Biao/Pictures/ade.jpg", "/Users/Biao/Pictures/avatar.jpg" });
    }

    {
        // [9] 共享 QNetworkAccessManager
        // 每创建一个 QNetworkAccessManager 对象都会创建一个线程，当频繁的访问网络时，为了节省线程资源，调用 manager()
        // 使用共享的 QNetworkAccessManager，它不会被 HttpClient 删除，需要我们自己不用的时候删除它。
        // 如果下面的代码不传入 QNetworkAccessManager，从任务管理器里可以看到创建了几千个线程。
        QNetworkAccessManager *manager = new QNetworkAccessManager();
        for (int i = 0; i < 5000; ++i) {
            HttpClient("http://localhost:8080/api/rest").manager(manager).success([=](const QString &response) {
                qDebug().noquote() << response << ", " << i;
            }).get();
        }
    }

    return a.exec();
}
