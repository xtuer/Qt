#ifndef CONFIGUTIL_H
#define CONFIGUTIL_H
#include <QString>
#include <QScopedPointer> // [1]

template <typename T> class Singleton; // [2]

class Config {
public:
    QString getDatabaseName() const;

private:
    // [3]
    Config();
    ~Config();
    Config(const Config &other);
    Config& operator=(const Config &other);

    // [4]
    friend class Singleton<Config>;
    friend struct QScopedPointerDeleter<Config>;
};

#endif // CONFIGUTIL_H
