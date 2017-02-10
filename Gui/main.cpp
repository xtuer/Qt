#include <QApplication>
#include <QDebug>
#include <QPushButton>
#include <QMovie>
#include <QLabel>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QLabel *label = new QLabel();
    QMovie *movie = new QMovie("/Users/Biao/Desktop/intro.gif");
    label->setMovie(movie);
    movie->start();
    label->show();

    QObject::connect(movie, &QMovie::frameChanged, [=](int frameNumber) {
        if (frameNumber == movie->frameCount() - 1) {
            movie->stop();
        }
    });

    return app.exec();
}
