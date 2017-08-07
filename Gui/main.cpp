#include <QApplication>
#include <QDebug>
#include "Widget.h"

#include <QUrl>
#include <QFile>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QHttpPart>
#include <QHttpMultiPart>


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QFile *file = new QFile("/Users/Biao/Pictures/ade.jpg");
    file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart
    file->open(QIODevice::ReadOnly);
    QString disposition = QString("form-data; name=\"file\"; filename=\"%1\"").arg(file->fileName());

    QHttpPart imagePart;
//    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file\"; filename=\"ade.jpg\""));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(disposition));
    imagePart.setBodyDevice(file);
    multiPart->append(imagePart);

    QNetworkRequest request(QUrl("http://localhost:8080/webuploader"));
    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.post(request, multiPart);
    multiPart->setParent(reply);

    QObject::connect(reply, &QNetworkReply::finished, [=] {
        qDebug() << reply->errorString();
        reply->deleteLater();
    });

    return a.exec();
}
