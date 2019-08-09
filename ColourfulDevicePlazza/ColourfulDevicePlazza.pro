#-------------------------------------------------
#
# Project created by QtCreator 2019-08-09T11:52:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG  += c++11
TARGET   = ColourfulDevicePlazza
TEMPLATE = app

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

SOURCES += \
        main.cpp \
        ColourfulDevicePlazza.cpp

HEADERS += \
        ColourfulDevicePlazza.h

FORMS += \
        ColourfulDevicePlazza.ui
