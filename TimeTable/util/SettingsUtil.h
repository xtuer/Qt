#ifndef SETTINGSUTIL_H
#define SETTINGSUTIL_H

#include "Singleton.h"
#include <QVariant>
#include <QSettings>

class TimeTableData;
class QSplitter;

class SettingsUtil {
    SINGLETON(SettingsUtil)
public:
    // 数据库信息
    static QString getDatabaseType();
    static QString getDatabaseHost();
    static QString getDatabaseName();
    static QString getDatabaseUsername();
    static QString getDatabasePassword();

    // Time Table Data
    static TimeTableData getTimeTableData();
    static void saveTimeTableData(const TimeTableData &data);
    static QString getTimeTableHtmlTemplate();

    static QString getStyleSheetFileName(); // 样式表文件.

    static void saveSplitterState(const QString &groupName, QSplitter *splitter);
    static void restoreSplitterState(const QString &groupName, QSplitter *splitter);
    static void saveWindowGeometry(const QString &groupName, QWidget *window);
    static void restoreWindowGeometry(const QString &groupName, QWidget *window);

private:
    static QVariant getAppValue(const  QString &groupName, const QString &settingName, const QVariant &def = QVariant());
    static void     setAppValue(const  QString &groupName, const QString &settingName, const QVariant &value);
    static QVariant getGuiValue(const  QString &groupName, const QString &settingName, const QVariant &def = QVariant());
    static void     setGuiValue(const  QString &groupName, const QString &settingName, const QVariant &value);
    static QVariant getDataValue(const QString &groupName, const QString &settingName, const QVariant &def = QVariant());
    static void     setDataValue(const QString &groupName, const QString &settingName, const QVariant &value);

    static QVariant getValue(QSettings *settings, const QString &groupName, const QString &settingName, const QVariant &def = QVariant());
    static void     setValue(QSettings *settings, const QString &groupName, const QString &settingName, const QVariant &value);

    QSettings *appSettings;
    QSettings *guiSettings;
    QSettings *dataSettings;
};

#endif // SETTINGSUTIL_H
 
