#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

void accessSqlite();

int main(int argc, char *argv[]) {
    Q_UNUSED(argc)
    Q_UNUSED(argv)

    accessSqlite();

    return 0;
}

void accessSqlite() {
    // [1] 加载 SQLite 数据库驱动
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Connection_Name");

    // [2] sqlitedata.db 是 SQLite 的数据库文件名
    //     如果此文件不存在则自动创建，如果已经存在则使用已经存在的文件
    db.setDatabaseName("sqlitedata.db");

    // [3] 打开数据库连接，成功返回 true，不成功则程序退出
    if (!db.open()) {
        qDebug() << "Connect to Sqlite error: " << db.lastError().text();
        exit(128);
    }

    // [4] 在数据库中创建表
    QSqlQuery createTableQuery(db);
    QString sql("CREATE TABLE user ("
                "id integer primary key autoincrement, "
                "username text not null, "
                "password text not null, "
                "email text, "
                "mobile text)");
    createTableQuery.exec(sql); // 如果表存在，则会创建表失败，但不会影响程序继续运行
    qDebug() << "Create table error: " << createTableQuery.lastError().text();

    // 向表中插入一条数据
    QSqlQuery insertQuery(db);
    insertQuery.exec("INSERT INTO user (username, password) VALUES ('Alice', 'passw0rd')");

    // 查询刚才插入的数据
    QSqlQuery selectQuery(db);
    selectQuery.exec("SELECT id, username, password FROM user");

    while (selectQuery.next()) {
        qDebug() << QString("Id: %1, Username: %2, Password: %3")
                    .arg(selectQuery.value("id").toInt())
                    .arg(selectQuery.value("username").toString())
                    .arg(selectQuery.value("password").toString());
    }
}
