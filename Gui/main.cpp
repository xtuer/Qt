#include <QSqlDatabase>
#include <QSqlQuery>
#include <QUrl>
#include <QDesktopServices>
#include <QApplication>
#include <QDebug>

void loadMySqlDriver();

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QDesktopServices::openUrl(QUrl("file:///Users/Biao/Desktop/ColorPicker", QUrl::TolerantMode));

    return app.exec();
}
