#-------------------------------------------------
#
# Project created by QtCreator 2019-03-16T20:56:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = TextTemplate
TEMPLATE = app

CONFIG  += c++11
CONFIG  -= app_bundle

DEFINES += QT_MESSAGELOGCONTEXT
DEFINES += QT_DEPRECATED_WARNINGS

# Output directory
CONFIG(debug, debug|release) {
    output = debug
    TARGET = $$TARGET'_d'
}
CONFIG(release, debug|release) {
    output = release
}

DESTDIR     = bin
OBJECTS_DIR = $$output
MOC_DIR     = $$output
RCC_DIR     = $$output
UI_DIR      = $$output

INCLUDEPATH += lib

SOURCES += \
        main.cpp \
        Widget.cpp

HEADERS += \
        Widget.h

FORMS += \
        Widget.ui
