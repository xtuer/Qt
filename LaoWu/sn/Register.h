#ifndef REGISTER_H
#define REGISTER_H

#include <QString>

class Register {
public:
    enum {Max_Time = 15};

    Register();
    virtual ~Register(); // 虚析构函数让子类在delete时析构函数能被正确调用.

    /**
     * @brief 当软件启动的时候调用此函数, 使用次数加1.
     */
    void usedAgain();

    /**
     * @brief 取得软件是否已经注册过.
     * @return 如果已经注册返回true, 否则返回false.
     */
    bool isRegistered();

    /**
     * @brief 取得软件是否已经过期. 如过期则不应再使用.
     * @return 如果已经过期返回true, 否则返回false.
     */
    bool isExpired();

    /**
     * @brief 如果传入的序列号是合法的，则保存到起来。
     * @param sn 序列号
     * @return 如果sn合法，则返回true，否则返回false.
     */
    bool registerSN(const QString &sn);

    /**
     * @brief 返回是否注册, 还有多少天使用期等信息
     * @return
     */
    QString getRegisterInfomation();

    /**
     * @brief 计算机的物理代码，例如使用网卡物理地址＋其他硬件信息
     * @return 返回计算机的物理代码
     */
    virtual QString getMachineCode();  // encode Mac address + platform dependent info: eg. cpu sn.

    static bool isSNValid(const QString &sn, const QString &machineCode);

protected:
    /**
     * @brief 读取与保存序列号
     * @return
     */
    virtual int  readUsedTimes() = 0;
    virtual void saveUsedTimes(int times) = 0;

    /**
     * @brief 读取与保存使用次数
     * @return
     */
    virtual QString readSN() = 0; // e.g. Win: read from register
    virtual void    saveSN(const QString &registerCode) = 0; // e.g. Win: save in register
};

#endif // REGISTER_H
 
