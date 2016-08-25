#ifndef USERDAO_H
#define USERDAO_H


class User;
class QString;
class QVariant;
template <typename T> class QList;
template <typename KT, typename VT> class QMap;

class UserDao {
public:
    static User selectById(int id);
    static QList<User> selectAll();

    static int  insert(const User &user);
    static bool update(const User &user);

private:
    static User mapToUser(const QMap<QString, QVariant> &rowMap);
};

#endif // USERDAO_H
