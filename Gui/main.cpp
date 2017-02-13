#include <QApplication>
#include <QMovie>
#include <QLabel>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QLabel *label = new QLabel();
    QMovie *movie = new QMovie("/Users/Biao/Desktop/x.gif");
    label->setMovie(movie); // 1. 设置要显示的 GIF 动画图片
    movie->start();         // 2. 启动动画
    label->show();

    QObject::connect(movie, &QMovie::frameChanged, [=](int frameNumber) {
        // GIF 动画执行一次就结束
        if (frameNumber == movie->frameCount() - 1) {
            movie->stop();
        }
    });

    return app.exec();
}
