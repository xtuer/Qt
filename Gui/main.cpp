#include <QApplication>
#include <QUrl>
#include <QDebug>
#include <QWebEnginePage>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWebEnginePage page;
    page.setUrl(QUrl("http://www.163.com"));

    return app.exec();
}
