#ifndef ROOM_H
#define ROOM_H

#include <QString>

class Room {
public:
    Room();
    QString toString() const;

    QString roomCode;
    QString roomLocation;
    int seatNum;
};

#endif // ROOM_H
