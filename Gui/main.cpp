#include "widget.h"

#include "HttpClient.h"

#include <QDebug>
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    {
        // 在代码块里执行网络访问，是为了测试 HttpClient 对象在被析构后，网络访问的回调函数仍然能正常执行
        HttpClient("http://localhost:8080/device").get([](const QString &response) {
            qDebug() << response;
        });

        HttpClient("http://localhost:8080/signIn")
                .addParam("id", "1")
                .addParam("name", "诸葛亮")
                .addHeader("token", "123AS#D")
                .get([](const QString &response) {
            qDebug() << response;
        });

        HttpClient("http://localhost:8080/signIn")
                .addParam("id", "2")
                .addParam("name", "卧龙")
                .addHeader("token", "DER#2J7")
                .addHeader("content-type", "application/x-www-form-urlencoded")
                .post([](const QString &response) {
            qDebug() << response;
        });
    }

    qDebug() << "------";
    return app.exec();
}
