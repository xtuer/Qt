#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QtCore/qglobal.h>

#if defined(BUILD_LIB)
#   define LIB Q_DECL_EXPORT
#else
#   define LIB Q_DECL_IMPORT
#endif

class LIB Calculator {
public:
    Calculator();
    int add(int a, int b) const;
};

LIB void work();

#endif // CALCULATOR_H
