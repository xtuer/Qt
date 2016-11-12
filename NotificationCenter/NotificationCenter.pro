#-------------------------------------------------
#
# Project created by QtCreator 2016-11-12T09:59:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NotificationCenter
TEMPLATE = app


SOURCES += main.cpp\
    NotificationCenter.cpp \
    FooObserver.cpp \
    BarObserver.cpp \
    Message.cpp \
    NotifyThread.cpp

HEADERS  += \
    NotificationCenter.h \
    FooObserver.h \
    BarObserver.h \
    Message.h \
    Singleton.h \
    NotifyThread.h

FORMS    +=
