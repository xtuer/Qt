#-------------------------------------------------
#
# Project created by QtCreator 2017-11-05T08:33:53
#
#-------------------------------------------------

QT      -= gui

TARGET   = MyLibrary
TEMPLATE = lib

DEFINES += MYLIBRARY_LIBRARY

# Output directory
CONFIG(debug, debug|release) {
    output = debug
}
CONFIG(release, debug|release) {
    output = release
}

DESTDIR     = ../bin
OBJECTS_DIR = $$output
MOC_DIR     = $$output
RCC_DIR     = $$output
UI_DIR      = $$output

SOURCES += \
    Calculator.cpp

HEADERS += \
    Calculator.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
