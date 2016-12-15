#ifndef CONFIGUTIL_H
#define CONFIGUTIL_H
#include <QString>
#include <QScopedPointer> // [1]

template <typename T> class Singleton; // [2]

class ConfigUtil {
public:
    QString getDatabaseName() const;

private:
    // [3]
    ConfigUtil();
    ~ConfigUtil();
    ConfigUtil(const ConfigUtil &other);
    ConfigUtil& operator=(const ConfigUtil &other);

    // [4]
    friend class Singleton<ConfigUtil>;
    friend struct QScopedPointerDeleter<ConfigUtil>;
};

#endif // CONFIGUTIL_H
