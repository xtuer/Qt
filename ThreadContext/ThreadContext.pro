#-------------------------------------------------
#
# Project created by QtCreator 2018-03-18T16:49:20
#
#-------------------------------------------------

QT       += core gui widgets
TARGET   = ThreadContext
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

HEADERS += \
    Widget.h \
    Thread.h

SOURCES += \
    main.cpp \
    Widget.cpp \
    Thread.cpp
