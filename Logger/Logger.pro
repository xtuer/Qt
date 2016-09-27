#-------------------------------------------------
#
# Project created by QtCreator 2016-09-20T16:56:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = Logger
TEMPLATE = app
CONFIG  -= app_bundle

# Output directory
CONFIG(debug, debug|release) {
    output = debug
}
CONFIG(release, debug|release) {
    output = release
}

DEFINES += QT_MESSAGELOGCONTEXT

DESTDIR     = bin
OBJECTS_DIR = $$output
MOC_DIR     = $$output
RCC_DIR     = $$output
UI_DIR      = $$output

SOURCES += main.cpp \
    LogHandler.cpp

HEADERS += \
    Singleton.h \
    LogHandler.h
