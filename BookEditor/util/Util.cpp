#include "Util.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>

bool Util::writeStringToFile(const QString &content, const QString &path, QString *error) {
    QFile file(path);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QString errorString = QString("保存文件 %1 出错：%2").arg(path).arg(file.errorString());
        qDebug().noquote() << errorString;

        if (0 != error) {
            *error = errorString;
        }

        return false;
    }

    QTextStream ts(&file);
    ts.setCodec(QTextCodec::codecForName("UTF8"));
    ts << content;
    file.flush();
    file.close();

    return true;
}
