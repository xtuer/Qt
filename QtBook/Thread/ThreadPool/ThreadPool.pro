#-------------------------------------------------
#
# Project created by QtCreator 2017-11-11T20:56:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ThreadPool
TEMPLATE = app

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

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
    Task.cpp

HEADERS += \
    Task.h

