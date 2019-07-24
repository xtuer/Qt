#ifndef CONFIG_H
#define CONFIG_H

#include "Singleton.h"
#include <QtCore/qglobal.h>

#if defined(BUILD_LIB)
#   define LIB Q_DECL_EXPORT
#else
#   define LIB Q_DECL_IMPORT
#endif

class LIB Config {
    SINGLETON(Config)

public:
    static int count;
};

LIB extern int g;

#endif // CONFIG_H
