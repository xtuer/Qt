#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QPluginLoader>
#include <QCoreApplication>
#include <QDebug>

void loadMySqlDriver();

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    loadMySqlDriver();
    return app.exec();
}

void loadMySqlDriver() {
    QPluginLoader loader;
    // MySQL 驱动插件的路径
    loader.setFileName("/Users/Biao/Qt5.8.0/5.8/clang_64/plugins/sqldrivers/libqsqlmysql.dylib");
    qDebug() << loader.load();
    qDebug() << loader.errorString();
}
