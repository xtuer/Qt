#-------------------------------------------------
#
# Project created by QtCreator 2017-11-19T17:22:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MouseSelection
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
        MouseSelectionWidget.cpp

HEADERS += \
        MouseSelectionWidget.h

FORMS +=
