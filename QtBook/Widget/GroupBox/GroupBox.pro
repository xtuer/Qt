#-------------------------------------------------
#
# Project created by QtCreator 2018-07-08T18:42:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GroupBox
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

SOURCES += \
        main.cpp \
    Widget.cpp

HEADERS += \
    Widget.h

FORMS += \
    Widget.ui

RESOURCES += \
    resources.qrc
