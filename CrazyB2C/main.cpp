#include "HttpClient.h"
#include <functional>
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDateTime>
#include <QThread>
#include <QNetworkAccessManager>

void increaseBuyOrFavorite(QNetworkAccessManager *manager, const QString &url);

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QFile file("urls.txt");
    if (!file.open(QIODevice::Text | QIODevice::ReadOnly)) {
        qDebug() << QString("打开 urls.txt 失败");
        return 1;
    }

    QNetworkAccessManager *manager = new QNetworkAccessManager();

    QTextStream in(&file);

    // 读取文件中的 URL
    while (!in.atEnd()) {
        QString url = in.readLine().trimmed();
        increaseBuyOrFavorite(manager, url);

//        QThread::msleep(1000);
    }

    return a.exec();
}

// 增加购买或者喜欢的数量
void increaseBuyOrFavorite(QNetworkAccessManager *manager, const QString &url) {
    if (url.isEmpty() || url.startsWith("#")) { return; }

    qint64 ms = QDateTime::currentDateTime().toMSecsSinceEpoch();
    qsrand(ms);
    int len = 30; // 预计增加 200 个
    int times = 0; // 成功增加的次数

    for (int i = 0; i < len; ++i) {
        // 60% 的增加机率
        if ((qrand() % 100) > 60) { continue; }
        times++;

        HttpClient(url).useManager(manager).get([&times](const QString &response) {
            Q_UNUSED(response)
        }, [=](const QString &error) {
            qDebug() << QString("Error: %1").arg(error);
        });
    }

    qDebug() << QString("%1, Times: %2").arg(url).arg(times);
}
