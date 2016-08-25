#include "RegisterFactory.h"

#include "Register.h"

#ifdef Q_OS_WIN
#include "WinRegister.h"
#elif defined Q_OS_MAC
#include "MacRegister.h"
#endif

Register& RegisterFactory::getRegister() {
#ifdef Q_OS_WIN
        return WinRegister::getInstance();
#elif defined Q_OS_MAC
        return MacRegister::getInstance();
#endif
}

