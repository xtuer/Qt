#include "SettingUtil.h"

#include <QString>
#include <QStringList>
#include <QSettings>
#include <QSplitter>
#include <QWidget>
#include <QDesktopWidget>

//----------------------------------------------------------------------//
//                    Definition of SettingUtilPrivate                  //
//----------------------------------------------------------------------//
class SettingUtilPrivate {
public:
    SettingUtilPrivate();
    ~SettingUtilPrivate();

    QVariant getAppValue(const  QString &groupName, const QString &settingName, const QVariant &def = QVariant());
    QVariant getGuiValue(const  QString &groupName, const QString &settingName, const QVariant &def = QVariant());
    QVariant getDataValue(const QString &groupName, const QString &settingName, const QVariant &def = QVariant());

    void     setAppValue(const  QString &groupName, const QString &settingName, const QVariant &value);
    void     setGuiValue(const  QString &groupName, const QString &settingName, const QVariant &value);
    void     setDataValue(const QString &groupName, const QString &settingName, const QVariant &value);

private:
    QVariant getValue(QSettings *settings, const QString &groupName, const QString &settingName, const QVariant &def = QVariant());
    void     setValue(QSettings *settings, const QString &groupName, const QString &settingName, const QVariant &value);

    QSettings *appSettings;
    QSettings *guiSettings;
    QSettings *dataSettings;
};

SettingUtilPrivate::SettingUtilPrivate() {
    appSettings = new QSettings("resources/app.ini", QSettings::IniFormat);
    guiSettings = new QSettings("resources/gui.ini", QSettings::IniFormat);
    dataSettings = new QSettings("resources/data.ini", QSettings::IniFormat);

    appSettings->setIniCodec("UTF-8");
    guiSettings->setIniCodec("UTF-8");
    dataSettings->setIniCodec("UTF-8");

    // qRegisterMetaTypeStreamOperators<TimeTableData>("TimeTableData");
}

SettingUtilPrivate::~SettingUtilPrivate() {
    appSettings->sync();
    guiSettings->sync();
    dataSettings->sync();

    delete appSettings;
    delete guiSettings;
    delete dataSettings;
}


QVariant SettingUtilPrivate::getAppValue(const QString& groupName, const QString& settingName, const QVariant& def) {
    return getValue(appSettings, groupName, settingName, def);
}

QVariant SettingUtilPrivate::getGuiValue(const QString& groupName, const QString& settingName, const QVariant& def) {
    return getValue(guiSettings, groupName, settingName, def);
}

QVariant SettingUtilPrivate::getDataValue(const QString& groupName, const QString& settingName, const QVariant& def) {
    return getValue(dataSettings, groupName, settingName, def);
}

void SettingUtilPrivate::setAppValue(const QString& groupName, const QString& settingName, const QVariant& value) {
    setValue(appSettings, groupName, settingName, value);
}

void SettingUtilPrivate::setGuiValue(const QString& groupName, const QString& settingName, const QVariant& value) {
    setValue(guiSettings, groupName, settingName, value);
}

void SettingUtilPrivate::setDataValue(const QString& groupName, const QString& settingName, const QVariant& value) {
    setValue(dataSettings, groupName, settingName, value);
}

QVariant SettingUtilPrivate::getValue(QSettings* settings,
                                      const QString& groupName,
                                      const QString& settingName,
                                      const QVariant& def) {
    QVariant var;

    settings->beginGroup(groupName);
    var = settings->value(settingName, def);
    settings->endGroup();

    return var;
}

void SettingUtilPrivate::setValue(QSettings* settings,
                                  const QString& groupName,
                                  const QString& settingName,
                                  const QVariant& value) {
    settings->beginGroup(groupName);
    settings->setValue(settingName, value);
    settings->endGroup();
    settings->sync();
}

//----------------------------------------------------------------------//
//                      Definition of SettingUtil                       //
//----------------------------------------------------------------------//
SINGLETON_INITIALIZE(SettingUtil)

#define GROUP_DATABASE "Database"
#define GROUP_APPLICATION "Application"

SettingUtil::SettingUtil() {
    data = new SettingUtilPrivate;
}

SettingUtil::~SettingUtil() {
    delete data;
}

QString SettingUtil::getDatabaseType() const {
    return data->getAppValue(GROUP_DATABASE, "database_type").toString().trimmed();
}

QString SettingUtil::getDatabaseHost() const {
    return data->getAppValue(GROUP_DATABASE, "database_host").toString().trimmed();
}

QString SettingUtil::getDatabaseName() const {
    return data->getAppValue(GROUP_DATABASE, "database_name").toString().trimmed();
}

QString SettingUtil::getDatabaseUsername() const {
    return data->getAppValue(GROUP_DATABASE, "database_username").toString().trimmed();
}

QString SettingUtil::getDatabasePassword() const {
    return data->getAppValue(GROUP_DATABASE, "database_password").toString().trimmed();
}

QStringList SettingUtil::getSqlFileNames() const {
    return data->getAppValue(GROUP_DATABASE, "sql_file_names").toStringList();
}

bool SettingUtil::isSqlDebug() const {
    return data->getAppValue(GROUP_DATABASE, "sql_debug", false).toBool();
}

QStringList SettingUtil::getQssFileNames() const {
    return data->getAppValue(GROUP_APPLICATION, "style_sheet_file_names").toStringList();
}

void SettingUtil::saveSplitterState(const QString &groupName, QSplitter *splitter) {
    data->setGuiValue(groupName, splitter->objectName(), splitter->saveState());
}

void SettingUtil::restoreSplitterState(const QString &groupName, QSplitter *splitter) const {
    splitter->restoreState(data->getGuiValue(groupName, splitter->objectName()).toByteArray());
}

void SettingUtil::saveWindowGeometry(const QString &groupName, QWidget *window) {
    // 保存窗口的左上角坐标和大小(不包含标题栏的大小)
    data->setGuiValue(groupName, "pos", window->pos());
    data->setGuiValue(groupName, "size", window->size());
}

void SettingUtil::restoreWindowGeometry(const QString &groupName, QWidget *window) const {
    QDesktopWidget *dw = new QDesktopWidget();
    QRect rect = dw->screenGeometry().marginsRemoved(QMargins(0, 0, 10, 10));
    QPoint pos = data->getGuiValue(groupName, "pos").toPoint();

    // 如果pos不在桌面窗口内, 让其左上角为(0, 0)
    if (!rect.contains(pos)) {
        pos = QPoint();
    }

    // 先移动左上角, 后修改大小
    window->move(pos);
    window->resize(data->getGuiValue(groupName, "size").toSize());
}
