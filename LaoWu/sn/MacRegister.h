#ifndef MACREGISTER_H
#define MACREGISTER_H

#include "Register.h"
#include "../util/Singleton.h"

class QString;
class QSettings;

class MacRegister : public Register {
    SINGLETON(MacRegister)

protected:
    virtual int  readUsedTimes();
    virtual void saveUsedTimes(int times);

    virtual QString readSN(); // e.g. Win: read from register
    virtual void    saveSN(const QString &sn); // e.g. Win: save in register

private:
    QSettings *settings;
};

#endif // MACREGISTER_H
 
