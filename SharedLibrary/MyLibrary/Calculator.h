#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QtCore/qglobal.h>

// 根据条件定义 MYLIBRARY_SHARED_SYMBOL 为不同的宏
#if defined(MYLIBRARY_LIBRARY)
#   define MYLIBRARY_SHARED_SYMBOL Q_DECL_EXPORT
#else
#   define MYLIBRARY_SHARED_SYMBOL Q_DECL_IMPORT
#endif

// 使用 MYLIBRARY_SHARED_SYMBOL 声明符号，编译时会根据条件替换为 Q_DECL_EXPORT 或者 Q_DECL_IMPORT
class MYLIBRARY_SHARED_SYMBOL Calculator {
public:
    Calculator();
    int add(int a, int b) const;
};

void MYLIBRARY_SHARED_SYMBOL work();

#endif // CALCULATOR_H
