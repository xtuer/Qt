include(../common.pri)

QT      += core gui widgets
TARGET   = Lib-2
TEMPLATE = lib

DEFINES += BUILD_LIB

LIBS += -L$$bin -lLib-1

FORMS += \
    Form.ui

HEADERS += \
    Form.h

SOURCES += \
    Form.cpp
