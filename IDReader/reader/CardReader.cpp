#include "CardReader.h"
#include "bean/Person.h"

extern "C" {
    #include "lib/CardReaderApi.h"
}

#include <QApplication>
#include <QLibrary>
#include <QDebug>
#include <QTextCodec>
#include <QThread>

CardReader::CardReader() {
    connected   = false;
    errorCode   = 0;
    errorString = "";
}

void CardReader::connect() {
    errorCode = 0;

    if (!connected) {
        errorCode    = ::OpenCardReader(0, 2, 115200);
        errorString  = readError();
        errorString += ""; // RtlWerpReportException without
        connected    = (0 == errorCode);
    }
}

void CardReader::disconnect() {
    errorCode = 0;

    if (connected) {
        errorCode   = ::CloseCardReader();
        errorString = readError();
        connected   = false;
    }
}

Person CardReader::read() {
    errorCode = 10000;

    if (connected) {
        wchar_t path[256];
        personImagePath().toWCharArray(path);
        PERSONINFOW p;

        errorCode = ::GetPersonMsgW(&p, path);
        errorString = readError();

        return Person(p);
    }

    return Person();
}

QString CardReader::personImagePath() {
    return QString("%1/person.bmp").arg(qApp->applicationDirPath());
}

QString CardReader::readError() {
    wchar_t error[128] = L"";
    ::GetErrorTextW(error, 128);

    return QString::fromWCharArray(error);
}

