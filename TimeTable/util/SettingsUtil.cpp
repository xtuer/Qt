#include "SettingsUtil.h"
#include "bean/TimeTableData.h"

#include <QWidget>
#include <QSplitter>
#include <QHeaderView>
#include <QDebug>
#include <QRegExp>
#include <QSplitter>
#include <QPoint>
#include <QSize>
#include <QVariant>
#include <QFile>

#define GROUP_SERVER "Server"
#define GROUP_DATABASE "Database"
#define GROUP_APPLICATION "Application"
#define GROUP_TIMETABLE "TimeTable"

SINGLETON_INITIALATE(SettingsUtil)

SettingsUtil::SettingsUtil() {
    appSettings = new QSettings("app.ini", QSettings::IniFormat);
    guiSettings = new QSettings("gui.ini", QSettings::IniFormat);
    dataSettings = new QSettings("data.ini", QSettings::IniFormat);

    appSettings->setIniCodec("UTF-8");
    guiSettings->setIniCodec("UTF-8");
    dataSettings->setIniCodec("UTF-8");

    qRegisterMetaTypeStreamOperators<TimeTableData>("TimeTableData");
}

SettingsUtil::~SettingsUtil() {
//    delete appSettings;
//    delete guiSettings;
    //delete dataSettings;
}

QString SettingsUtil::getDatabaseType() {
    return getAppValue(GROUP_DATABASE, "database_type").toString().trimmed();
}

QString SettingsUtil::getDatabaseHost() {
    return getAppValue(GROUP_DATABASE, "database_host").toString().trimmed();
}

QString SettingsUtil::getDatabaseName() {
    return getAppValue(GROUP_DATABASE, "database_name").toString().trimmed();
}

QString SettingsUtil::getDatabaseUsername() {
    return getAppValue(GROUP_DATABASE, "database_username").toString().trimmed();
}

QString SettingsUtil::getDatabasePassword() {
    return getAppValue(GROUP_DATABASE, "database_password").toString().trimmed();
}

TimeTableData SettingsUtil::getTimeTableData() {
    QVariant data = getDataValue(GROUP_TIMETABLE, "timeTableData");
    return data.isValid() ? data.value<TimeTableData>() : TimeTableData();
}

void SettingsUtil::saveTimeTableData(const TimeTableData& data) {
    setDataValue(GROUP_TIMETABLE, "timeTableData", QVariant::fromValue(data));
}

QString SettingsUtil::getTimeTableHtmlTemplate() {
    QFile file("printTemplate.html");

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Cannot open file printTemplate.html";
        return "";
    }

    QString htmlTemplate = file.readAll();
    file.close();

    return htmlTemplate;
}

QString SettingsUtil::getStyleSheetFileName() {
    return getAppValue(GROUP_APPLICATION, "style_sheet_file_name", "style.css").toString().trimmed();
}

void SettingsUtil::saveSplitterState(const QString &groupName, QSplitter *splitter) {
    setGuiValue(groupName, splitter->objectName(), splitter->saveState());
}

void SettingsUtil::restoreSplitterState(const QString &groupName, QSplitter *splitter) {
    splitter->restoreState(getGuiValue(groupName, splitter->objectName()).toByteArray());
}

void SettingsUtil::saveWindowGeometry(const QString &groupName, QWidget *window) {
    setGuiValue(groupName, "pos", window->pos());
    setGuiValue(groupName, "size", window->rect().size());
}

void SettingsUtil::restoreWindowGeometry(const QString &groupName, QWidget *window) {
    window->move(getGuiValue(groupName, "pos").toPoint());
    window->resize(getGuiValue(groupName, "size").toSize());
}

//////////////////////////////////////////////////
///                                             //
///                 Private Methods             //
///                                             //
//////////////////////////////////////////////////

QVariant SettingsUtil::getAppValue(const QString &groupName, const QString &settingName, const QVariant &def) {
    return getValue(getInstance().appSettings, groupName, settingName, def);
}

void SettingsUtil::setAppValue(const QString &groupName, const QString &settingName, const QVariant &value) {
    setValue(getInstance().appSettings, groupName, settingName, value);
}

QVariant SettingsUtil::getGuiValue(const QString& groupName, const QString& settingName, const QVariant& def) {
    return getValue(getInstance().guiSettings, groupName, settingName, def);
}

void SettingsUtil::setGuiValue(const QString& groupName, const QString& settingName, const QVariant& value) {
    setValue(getInstance().guiSettings, groupName, settingName, value);
}

QVariant SettingsUtil::getDataValue(const QString& groupName, const QString& settingName, const QVariant &def) {
    return getValue(getInstance().dataSettings, groupName, settingName, def);
}

void SettingsUtil::setDataValue(const QString& groupName, const QString& settingName, const QVariant& value) {
    setValue(getInstance().dataSettings, groupName, settingName, value);
}

QVariant SettingsUtil::getValue(QSettings* settings, const QString& groupName, const QString& settingName, const QVariant &def) {
    QVariant var;

    settings->beginGroup(groupName);
    var = settings->value(settingName, def);
    settings->endGroup();

    return var;
}

void SettingsUtil::setValue(QSettings* settings,
                            const QString& groupName,
                            const QString& settingName,
                            const QVariant& value) {
    settings->beginGroup(groupName);
    settings->setValue(settingName, value);
    settings->endGroup();
    settings->sync();
}
