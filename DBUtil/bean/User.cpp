#include "User.h"

QString User::toString() const {
    return QString("ID: %1, Username: %2, Password: %3, Email: %4, Mobile: %5")
            .arg(id).arg(username).arg(password).arg(email).arg(mobile);
}
