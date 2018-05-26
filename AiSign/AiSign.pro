#-------------------------------------------------
#
# Project created by QtCreator 2018-05-25T13:43:40
#
#-------------------------------------------------

QT       += core gui network multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AiSign
TEMPLATE = app

CONFIG  -= app_bundle
DEFINES += QT_MESSAGELOGCONTEXT
DEFINES += QT_DEPRECATED_WARNINGS

# Output directory
CONFIG(debug, debug|release) {
    output = debug
}
CONFIG(release, debug|release) {
    output = release
}

DESTDIR     = AiSign
OBJECTS_DIR = $$output
MOC_DIR     = $$output
RCC_DIR     = $$output
UI_DIR      = $$output

include(gui/gui.pri)
include(util/util.pri)

SOURCES += main.cpp

