#-------------------------------------------------
#
# Project created by QtCreator 2018-09-05T08:53:36
#
#-------------------------------------------------

QT += core gui widgets

TARGET   = zbig
TEMPLATE = app

# Defines
DEFINES += QT_MESSAGELOGCONTEXT

# Output directory
CONFIG(debug, debug|release) {
    output = debug
    TARGET = zbig_d
}
CONFIG(release, debug|release) {
    output = release
}

DESTDIR     = bin
OBJECTS_DIR = $$output
MOC_DIR     = $$output
RCC_DIR     = $$output
UI_DIR      = $$output

# [1] Define include path to src
INCLUDEPATH += ../src

# [2] Include module relative to src
include(../src/gui/gui.pri)
include(../src/bean/bean.pri)

SOURCES += main.cpp
