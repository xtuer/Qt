#include "Room.h"

Room::Room() {
    seatNum = 0;
}

QString Room::toString() const {
    return QString("roomCode: %1, roomLocation: %2, seatNum: %3").arg(roomCode).arg(roomLocation).arg(seatNum);
}
