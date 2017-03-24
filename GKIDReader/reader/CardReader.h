#ifndef CARDREADER_H
#define CARDREADER_H

#include <QString>

class Person;

class CardReader {
public:
    CardReader();

    void connect();
    void disconnect();
    Person read();
    static QString personImagePath();

    bool    connected;
    qint32  errorCode;
    QString errorString;

private:
    QString readError();
};

#endif // CARDREADER_H
