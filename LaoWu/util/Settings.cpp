#include "Settings.h"
#include <QString>
#include <QSettings>
#include <QWidget>
#include <QSplitter>
#include <QHeaderView>

Settings* Settings::instance = 0;

Settings& Settings::getInstance() {
    if (0 == instance) {
        instance = new Settings();
    }

    return *instance;
}

void Settings::release() {
    if (0 != instance) {
        delete instance;
        instance = 0;
    }
}

void Settings::saveWindowBounds(const QWidget &widget, const QString &settingName) {
    QRect rect(widget.frameGeometry().topLeft(), widget.geometry().size());
    settings->setValue(settingName, rect);
    settings->sync();
}

void Settings::restoreWindowBounds(QWidget *widget, const QString &settingName) {
    QRect rect = settings->value(settingName, QRect(0, 0, 640, 480)).toRect();
    widget->move(rect.topLeft());
    widget->resize(rect.size());
}

void Settings::saveSplitterState(const QSplitter &splitter, const QString &settingName) {
    settings->setValue(settingName, splitter.saveState());
}

void Settings::restoreSplitterState(QSplitter *splitter, const QString &settingName) {
    splitter->restoreState(settings->value(settingName).toByteArray());
}

void Settings::saveHeaderState(const QHeaderView &headerView, const QString &settingName) {
    settings->setValue(settingName, headerView.saveState());
}

void Settings::restoreHeaderState(QHeaderView *headerView, const QString &settingName) {
    headerView->restoreState(settings->value(settingName).toByteArray());
}

void Settings::saveApplicationFontSize(int fontSize) {
    settings->setValue("Application/fontSize", fontSize);
}

int Settings::getApplicationFontSize() {
    return settings->value("Application/fontSize", 15).toInt();
}

void Settings::saveToolTipFontSize(int fontSize) {
    settings->setValue("Application/toolTipFontSize", fontSize);
}

int Settings::getToolTipFontSize() {
    return settings->value("Application/toolTipFontSize", 13).toInt();
}

void Settings::saveStyleName(const QString &styleName) {
    settings->setValue("Application/styleName", styleName);
}

QString Settings::getStyleName() const {
    return settings->value("Application/styleName", "WindowsXP").toString();
}

void Settings::saveShopAddress(const QString &shopAddress) {
    settings->setValue("Shop/Address", shopAddress);
}

QString Settings::getShopAddress() {
    return settings->value("Shop/Address").toString();
}

void Settings::saveShopTelephone(const QString &shopTelephone) {
    settings->setValue("Shop/Telephone", shopTelephone);
}

QString Settings::getShopTelephone() {
    return settings->value("Shop/Telephone").toString();
}

void Settings::saveShopExtraInfo(const QString &shopExtraInfo) {
    settings->setValue("Shop/ExtraInfo", shopExtraInfo);
}

QString Settings::getShopExtraInfo() {
    return settings->value("Shop/ExtraInfo").toString();
}

void Settings::saveTtsEnabled(bool enabled) {
    settings->setValue("TtsEnabled", enabled);
}

bool Settings::getTtsEnabled() {
    return settings->value("TtsEnabled", true).toBool();
}

void Settings::saveVerticalHeaderVisible(const QString &tableViewName, bool visible) {
    settings->setValue(tableViewName, visible);
}

bool Settings::isVerticalHeaderVisible(const QString &tableViewName) {
    return settings->value(tableViewName, false).toBool();
}

void Settings::saveFineryIdVisible(bool visible) {
    settings->setValue("Finery", visible);
}

bool Settings::isFineryIdVisible() {
    return settings->value("Finery", false).toBool();
}

// Print
void Settings::savePrintFontName(const QString &fontName) {
    settings->setValue("Print/FontName", fontName);
}

void Settings::savePrintFontPointSize(int pointSize) {
    settings->setValue("Print/PointSize", pointSize);
}

void Settings::savePrintFontBold(bool bold) {
    settings->setValue("Print/Bold", bold);
}

QString Settings::getPrintFontName() {
    return settings->value("Print/FontName", "宋体").toString();
}

int Settings::getPrintFontPointSize() {
    return settings->value("Print/PointSize", 10).toInt();
}

bool Settings::isPrintFontBold() {
    return settings->value("Print/Bold", true).toBool();
}

void Settings::savePrintPaperWidth(int width) {
    settings->setValue("Print/PaperWidth", width);
}

int Settings::getPrintPaperWidth() {
    return settings->value("Print/PaperWidth", 58).toInt();
}

QString Settings::getBackupDirectory() {
    return settings->value("backup", "backup").toString();
}

void Settings::setBackupDirectory(const QString &path) {
    settings->setValue("backup", path);
}

QString Settings::getPassword() {
    return settings->value("password", "202cb962ac59075b964b07152d234b70").toString(); // 默认密码是 123
}

void Settings::setPassword(const QString &password) {
    settings->setValue("password", password);
}

Settings::Settings() {
    settings = new QSettings("app.ini", QSettings::IniFormat);
}

Settings::~Settings() {
    settings->sync();
    delete settings;
}
