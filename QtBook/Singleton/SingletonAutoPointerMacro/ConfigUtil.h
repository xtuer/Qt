#ifndef CONFIGUTIL_H
#define CONFIGUTIL_H

#include "Singleton.h" // [1]
#include <QString>

class ConfigUtil {
    SINGLETON(ConfigUtil) // [2]

public:
    QString getDatabaseName() const;
};

#endif // CONFIGUTIL_H
