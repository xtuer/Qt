#ifndef ROOM_H
#define ROOM_H

#include <QString>

/**
 * 考场: 机房
 */
class Room {
public:
    Room();
    QString toString() const;

    QString roomCode;
    QString roomLocation;
    int  seatNum;
    bool needSignIn = false;
};

#endif // ROOM_H
