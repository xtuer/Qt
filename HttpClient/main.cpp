#include "HttpClient.h"

#include <QDebug>
#include <QFile>
#include <QApplication>
#include <QNetworkAccessManager>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // 在代码块里执行网络访问，是为了测试 HttpClient 对象在被析构后，网络访问的回调函数仍然能正常执行
    {
        QString url("http://localhost:8080/rest");

        // [[1]] GET 请求无参数
        HttpClient(url).get([](const QString &response) {
            qDebug().noquote() << response;
        });

        // [[2]] GET 请求有参数，有自定义 header
        HttpClient(url).debug(true).param("name", "诸葛亮").header("token", "md5sum").get([](const QString &response) {
            qDebug().noquote() << response;
        });

        // [[3]] POST 请求，使用 param 添加参数，请求的参数使用 Form 格式
        HttpClient(url).debug(true).param("name", "卧龙")
                .post([](const QString &response) {
            qDebug().noquote() << response;
        });

        // [[4]] PUT 请求，使用 json 添加参数，请求的参数使用 Json 格式
        HttpClient(url).debug(true).json("{\"name\": \"孔明\"}").put([](const QString &response) {
            qDebug().noquote() << response;
        });

        // [[5]] DELETE 请求
        HttpClient(url).debug(true).remove([](const QString &response) {
            qDebug().noquote() << response;
        });

        // [[6]] 下载: 直接保存到文件
        HttpClient("http://xtuer.github.io/img/dog.png").debug(true).download("/Users/Biao/Desktop/dog-1.png");

        // [[7]] 下载: 自己处理下载得到的字节数据
        QFile *file = new QFile("/Users/Biao/Desktop/dog-2.png");
        if (file->open(QIODevice::WriteOnly)) {
            HttpClient("http://xtuer.github.io/img/dog.png").debug(true).download([=](const QByteArray &data) {
                file->write(data);
            }, [=](const QString &) {
                file->flush();
                file->close();
                file->deleteLater();

                qDebug().noquote() << "下载完成";
            });
        } else {
            file->deleteLater();
            file = NULL;
        }

        // [[8]] 上传文件
        HttpClient("http://localhost:8080/upload").debug(true).upload(QString("/Users/Biao/Pictures/ade.jpg"));

        // [[9]] 上传文件: 也能同时传参数
        HttpClient("http://localhost:8080/upload").debug(true)
                .param("username", "Alice").param("password", "Passw0rd")
                .upload(QString("/Users/Biao/Pictures/ade.jpg"));

        // [[10]] 上传数据: 例如使用摄像头拍照后直接把图片数据传到服务器
        QFile dataFile("/Users/Biao/Pictures/ade.jpg");
        dataFile.open(QIODevice::ReadOnly);
        QByteArray data = dataFile.readAll();
        HttpClient("http://localhost:8080/upload").debug(true).upload(data);
    }

    {
        // [[11]] 共享 QNetworkAccessManager
        // 每创建一个 QNetworkAccessManager 对象都会创建一个线程，当频繁的访问网络时，为了节省线程资源，调用 manager()
        // 使用共享的 QNetworkAccessManager，它不会被 HttpClient 删除，需要我们自己不用的时候删除它。
        // 如果下面的代码不传入 QNetworkAccessManager，从任务管理器里可以看到创建了几千个线程。
        QNetworkAccessManager *manager = new QNetworkAccessManager();
        for (int i = 0; i < 5000; ++i) {
            HttpClient("http://localhost:8080/rest").manager(manager).get([=](const QString &response) {
                qDebug().noquote() << response << ", " << i;
            });
        }
    }

    return a.exec();
}
