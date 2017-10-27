#include "HttpClient.h"

#include <QDebug>
#include <QFile>
#include <QApplication>
#include <QNetworkAccessManager>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // 在代码块里执行网络访问，是为了测试 HttpClient 对象在被析构后，网络访问的回调函数仍然能正常执行
    {
        // [[1]] GET 请求无参数
        HttpClient("http://localhost:8080/rest").get([](const QString &response) {
            qDebug() << response;
        });

        // [[2]] GET 请求有参数，有自定义 header
        HttpClient("http://localhost:8080/rest").debug(true)
                .addParam("name", "诸葛亮")
                .addHeader("token", "123AS#D")
                .get([](const QString &response) {
            qDebug() << response;
        });

        // [[3]] POST 请求，使用 addParam 添加参数时，请求的参数使用 Form 格式
        HttpClient("http://localhost:8080/rest").debug(true)
                .addParam("name", "卧龙")
                .post([](const QString &response) {
            qDebug() << response;
        });

        // [[4]] POST 请求，使用 jsonData 添加参数时，请求的参数使用 Json 格式
        HttpClient("http://localhost:8080/restJson").debug(true)
                .jsonData("{\"name\": \"Alice\"}")
                .post([](const QString &response) {
            qDebug() << response;
        });

        // [[5]] 下载: 下载直接保存到文件
        HttpClient("http://xtuer.github.io/img/dog.png").debug(true).download("/Users/Biao/Desktop/dog.png");

        // [[6]] 下载: 自己处理下载得到的字节数据
        QFile *file = new QFile("dog.png");
        if (file->open(QIODevice::WriteOnly)) {
            HttpClient("http://xtuer.github.io/img/dog.png").debug(true).download([=](const QByteArray &data) {
                file->write(data);
            }, [=] {
                file->flush();
                file->close();
                file->deleteLater();

                qDebug() << "下载完成";
            });
        }

        // [[7]] 上传
        HttpClient("http://localhost:8080/webuploader").upload("/Users/Biao/Pictures/ade.jpg");
    }

    {
        // [[8]] 共享 QNetworkAccessManager
        // 每创建一个 QNetworkAccessManager 对象都会创建一个线程，当频繁的访问网络时，为了节省线程资源，调用 useManager()
        // 使用共享的 QNetworkAccessManager，它不会被 HttpClient 删除，需要我们自己不用的时候删除它。
        // 如果下面的代码不传入 QNetworkAccessManager，从任务管理器里可以看到创建了几千个线程。
        QNetworkAccessManager *manager = new QNetworkAccessManager();
        for (int i = 0; i < 5000; ++i) {
            HttpClient("http://localhost:8080/rest").useManager(manager).get([=](const QString &response) {
                qDebug() << response << ", " << i;
            });
        }
    }

    return a.exec();
}
