#include <QApplication>
#include <QDebug>
#include "Widget.h"
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QUrl>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QMediaPlayer *player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile("/Users/Biao/Desktop/x.mp4"));

    QVideoWidget *videoWidget = new QVideoWidget;
    player->setVideoOutput(videoWidget);
    videoWidget->resize(400, 400);
    videoWidget->show();

    player->play();

    return app.exec();
}
