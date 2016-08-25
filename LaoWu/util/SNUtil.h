#ifndef SNUTIL_HPP
#define SNUTIL_HPP

class QString;

class SNUtil {
public:
    /**
     * @brief 使用自定义算法，加密字符串
     * @param sn
     * @return
     */
    static QString encryptSN(const QString &sn);

    /**
     * @brief 格式化字符串，使其更容易读: ABCD-32AD-GDAD-GODL
     * @param sn
     * @return
     */
    static QString formatSN(const QString &sn);

    /**
     * @brief 取得网卡的物理地址
     * @return 返回网卡的物理地址
     */
    static QString getMacAddress();

    /**
     * @brief 使用机器码生成序列号.
     * @param machineCode 机器码.
     * @return 返回序列号.
     */
    static QString generateSN(const QString &machineCode);
};

#endif // SNUTIL_HPP
