#ifndef SETTINGS_H
#define SETTINGS_H

#include <memory>

class QSettings;
class QWidget;
class QSplitter;
class QString;
class QHeaderView;

class Settings {
public:
    static Settings& getInstance();

    void saveWindowBounds(const QWidget &widget, const QString &settingName);
    void restoreWindowBounds(QWidget *widget, const QString &settingName);

    void saveSplitterState(const QSplitter &splitter, const QString &settingName);
    void restoreSplitterState(QSplitter *splitter, const QString &settingName);

    void saveHeaderState(const QHeaderView &headerView, const QString &settingName);
    void restoreHeaderState(QHeaderView *headerView, const QString &settingName);

    void saveApplicationFontSize(int fontSize);
    int getApplicationFontSize();

    void saveToolTipFontSize(int fontSize);
    int getToolTipFontSize();

    void saveStyleName(const QString &styleName);
    QString getStyleName() const;

    void saveShopAddress(const QString &shopAddress);
    QString getShopAddress();
    void saveShopTelephone(const QString &shopTelephone);
    QString getShopTelephone();
    void saveShopExtraInfo(const QString &shopExtraInfo);
    QString getShopExtraInfo();

    void saveTtsEnabled(bool enabled);
    bool getTtsEnabled();

    void saveVerticalHeaderVisible(const QString &tableViewName, bool visible);
    bool isVerticalHeaderVisible(const QString &tableViewName);

    void saveFineryIdVisible(bool visible);
    bool isFineryIdVisible();

    // Print font
    void savePrintFontName(const QString &fontName);
    void savePrintFontPointSize(int pointSize);
    void savePrintFontBold(bool bold);
    QString getPrintFontName();
    int getPrintFontPointSize();
    bool isPrintFontBold();
    void savePrintPaperWidth(int width);
    int getPrintPaperWidth();

    QString getBackupDirectory();
    void setBackupDirectory(const QString &path);

    QString getPassword();
    void setPassword(const QString &password);

    static void release();

private:
    Settings();
    ~Settings();
    Settings(const Settings &other);
    Settings& operator=(const Settings &other);

    QSettings *settings;

    friend class std::auto_ptr<Settings>;
    static Settings *instance; // ConfigUtil 全局唯一的变量
};

#endif // SETTINGS_H
