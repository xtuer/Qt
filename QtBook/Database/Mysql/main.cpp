#include <QCoreApplication>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QPluginLoader>

void accessMySql();

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    accessMySql();
    return app.exec();
}

void accessMySql() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "Connection_Name");
    db.setDatabaseName("qt");
    db.setHostName("127.0.0.1");
    db.setUserName("root");
    db.setPassword("root");

    if (!db.open()) {
        qDebug() << "Connect to MySql error: " << db.lastError().text();
        return;
    }

    QSqlQuery query(db);
    query.exec("SELECT * FROM user");

    while (query.next()) {
        qDebug() << query.value("username").toString();
    }
}
