#ifndef USERDATA_H
#define USERDATA_H

#include <QString>
#include <QStringList>

class UserData {
public:
    QString leiXin;     // 类别
    QString shangQuan;  // 商券
    QString guiShu;     // 归属
    QString zhangHao;   // 帐号
    QString jiaoYiMiMa; // 交易密码
    QString benJing;    // 本金: 5
    QString huMing;     // 户名
    QString daoJiShi;   // 倒计时
    QString kaiShiRi;   // 开始日
    QString zhiFuDao;   // 支付到
    QString liXi;       // 利息
    QString yinFuLiXi;   // 应付利息: 11
    QString ShiFuLiXi;  // 实付利息
    QString zhiJingMiMa;// 资金密码
    QString biLi;       // 比利
    QString yongJing;   // 佣金
    QString zhuangTai;  // 状态
    QString beiZhu;     // 备注

    QString reserved1;  // 预留1
    QString reserved2;  // 预留2
    QString reserved3;  // 预留3
    QString reserved4;  // 预留4
    QString reserved5;  // 预留5
    QString reserved6;  // 预留6

    static QStringList fieldLabels;
};

#endif // USERDATA_H
