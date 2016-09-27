#include "Site.h"
#include "Room.h"

Site::Site() {

}

QString Site::toString() const {
    QString s1 = QString("siteCode: %1, siteName: %2, siteAddress: %3, ").arg(siteCode).arg(siteName).arg(siteAddress);

    QString s2 = "rooms: [";
    for (int i = 0; i < rooms.size() - 1; ++i) {
        s2 += rooms.at(i).toString() + ", ";
    }

    if (rooms.size() > 0) {
        s2 += rooms.last().toString();
    }

    s2 += "]";

    return s1 + s2;
}
