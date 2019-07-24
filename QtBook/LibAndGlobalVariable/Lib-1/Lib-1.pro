include(../common.pri)

QT      += core
QT      -= gui
TARGET   = Lib-1
TEMPLATE = lib

DEFINES += BUILD_LIB

SOURCES += \
    Config.cpp

HEADERS += \
    Config.h \
    Singleton.h
