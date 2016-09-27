#ifndef CARDREADERAPI_H
#define CARDREADERAPI_H

struct PERSONINFOW {
    wchar_t name[16];
    wchar_t gender[2];
    wchar_t nationality[10];
    wchar_t birthday[10];
    wchar_t address[36];
    wchar_t cardId[20];
    wchar_t police[16];
    wchar_t validStart[10];
    wchar_t validEnd[10];
    wchar_t sexCode[2];
    wchar_t nationCode[4];
    wchar_t appendMsg[36];
};

qint32 __stdcall OpenCardReader(qint32 lPort, quint64 ulFlag, quint64 ulBaudRate);
qint32 __stdcall CloseCardReader();
qint32 __stdcall GetPersonMsgW(PERSONINFOW *pInfo, const wchar_t* pszImageFile);
void   __stdcall GetErrorTextW(wchar_t* pszBuffer, quint32 dwBufLen);

#endif // CARDREADERAPI_H
