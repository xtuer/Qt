#-------------------------------------------------
#
# Project created by QtCreator 2016-11-23T13:25:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FlatBuffer
TEMPLATE = app

include(flatbuffers/flatbuffers.pri)

SOURCES += main.cpp

HEADERS  += \
    Person_generated.h

FORMS    +=
