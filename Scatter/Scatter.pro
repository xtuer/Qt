#-------------------------------------------------
#
# Project created by QtCreator 2018-05-31T20:07:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Scatter
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
CONFIG  -= app_bundle

# Output directory
CONFIG(debug, debug|release) {
    output = debug
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
    Scatter.cpp \
    ScatterMap.cpp \
    Widget.cpp

HEADERS += \
    Scatter.h \
    ScatterMap.h \
    Widget.h

FORMS += \
    Widget.ui
