#include "Global.h"
#include "Settings.h"
#include <QDir>
#include <QFile>
#include <QDateTime>

double Global::stringToDouble(const QString &str, const QString &suffix) {
    return str.mid(0, str.indexOf(suffix)).toDouble();
}


int Global::stringToInt(const QString &str, const QString &suffix) {
    return str.mid(0, str.indexOf(suffix)).toInt();
}

void Global::backup() {
    QString prefix = QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss"); // 时间作为前缀
    QString backupFolder = Settings::getInstance().getBackupDirectory();

    QString newDataFile  = QString("%1/%2_%3").arg(backupFolder).arg(prefix).arg(Global::FILE_NAME_DATA);
    QString newNotesFile = QString("%1/%2_%3").arg(backupFolder).arg(prefix).arg(Global::FILE_NAME_NOTES);

    // 备份文件夹不存在则创建
    // 删除存在的同名备份文件
    QDir dir(".");
    if (!dir.exists(backupFolder))   { dir.mkdir(backupFolder); }
    if (QFile::exists(newDataFile))  { QFile::remove(newDataFile); }
    if (QFile::exists(newNotesFile)) { QFile::remove(newNotesFile); }

    // 备份文件
    QFile::copy(Global::FILE_NAME_DATA,  newDataFile);
    QFile::copy(Global::FILE_NAME_NOTES, newNotesFile);
}
