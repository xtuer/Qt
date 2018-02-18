#-------------------------------------------------
#
# Project created by QtCreator 2018-02-18T13:28:53
#
#-------------------------------------------------

QT      += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = TopWindow
TEMPLATE = app

CONFIG  -= app_bundle

# Output directory
CONFIG(debug, debug|release) {
    output = debug
}
CONFIG(release, debug|release) {
    output = release
}

include(gui/gui.pri)
include(util/util.pri)

DESTDIR     = bin
OBJECTS_DIR = $$output
MOC_DIR     = $$output
RCC_DIR     = $$output
UI_DIR      = $$output

SOURCES += \
        main.cpp

RESOURCES += \
    top-window-resources.qrc
