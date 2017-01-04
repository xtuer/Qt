#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

class Config {
public:
    static QString getQssFilePath();
    static QString getConfigFilePath();
};

#endif // CONFIG_H
