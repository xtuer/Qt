#-------------------------------------------------
#
# Project created by QtCreator 2016-09-01T12:10:48
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IDReader
TEMPLATE = app

RC_ICONS = AppIcon.ico

DEFINES += QT_MESSAGELOGCONTEXT

#Output directory
CONFIG(debug, debug|release) {
    compiled = debug
}

CONFIG(release, debug|release) {
    compiled = release
}

DESTDIR     = bin
OBJECTS_DIR = $$compiled
MOC_DIR     = $$compiled
RCC_DIR     = $$compiled
UI_DIR      = $$compiled

include(gui/gui.pri)

SOURCES += main.cpp

RESOURCES += \
    icon.qrc

