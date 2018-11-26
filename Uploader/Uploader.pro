#-------------------------------------------------
#
# Project created by QtCreator 2018-11-20T16:39:09
#
#-------------------------------------------------

QT       += core gui widgets network

TARGET = Uploader
TEMPLATE = app
RC_ICONS = AppIcon.ico

CONFIG  -= app_bundle
CONFIG += c++11
DEFINES += QT_MESSAGELOGCONTEXT
DEFINES += QT_DEPRECATED_WARNINGS

# Output directory
CONFIG(debug, debug|release) {
    output = debug
    TARGET = Uploader_d
}
CONFIG(release, debug|release) {
    output = release
}

DESTDIR     = bin
OBJECTS_DIR = $$output
MOC_DIR     = $$output
RCC_DIR     = $$output
UI_DIR      = $$output

include(gui/gui.pri)
include(util/util.pri)

SOURCES += \
        main.cpp


