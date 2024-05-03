#ifndef CONFIG_H
#define CONFIG_H
#include <QString>

namespace Config {
/**
 * 服务器地址（可放配置文件）
 */
const QString serverUrl = "http://localhost:8080"; // 线上环境: "http://118.195.181.149:8082";

/**
 * 机构的应用 ID，OAuth 登录使用（可放配置文件）
 */
const QString appId = "1653608449189";

/**
 * 机构的应用 Key，OAuth 登录使用（可放配置文件）
 */
const QString appKey = "01632b2e68882bf8bdb154f16d500b3b";

/**
 * 登录地址
 */
const QString URL_LOGIN = QString("%1/api/login/oauth").arg(serverUrl);

/**
 * 修改权限的地址
 */
const QString URL_PERMISSION_CHANGE = QString("%1/api/users/{userId}").arg(serverUrl);

/**
 * 修改用户密码的地址
 */
const QString URL_PASSWORD_CHANGE = QString("%1/api/users/{userId}/password").arg(serverUrl);

/**
 * 机构的用户
 */
const QString URL_USERS_OF_ORG = QString("%1/api/orgs/{orgId}/users").arg(serverUrl);
}

#endif // CONFIG_H
