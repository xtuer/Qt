#ifndef WINREGISTER_H
#define WINREGISTER_H

#include "Register.h"
#include "../util/Singleton.h"

class QString;
class QSettings;

class WinRegister : public Register {
    SINGLETON(WinRegister)

protected:
    virtual int  readUsedTimes();
    virtual void saveUsedTimes(int times);

    virtual QString readSN(); // e.g. Win: read from register
    virtual void    saveSN(const QString &sn); // e.g. Win: save in register

    virtual QString getMachineCode();

private:
    QSettings *snSettings;
    QSettings *usedTimesSettings;
};

#endif // WINREGISTER_H
 
