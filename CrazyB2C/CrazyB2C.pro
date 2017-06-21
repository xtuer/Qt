#-------------------------------------------------
#
# Project created by QtCreator 2014-08-27T22:30:14
#
#-------------------------------------------------

QT       += core gui xml sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CrazyB2C
TEMPLATE = app
CONFIG  -= app_bundle
ICON     = AppIcon.icns
RC_ICONS = AppIcon.ico

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

include (db/db.pri)
include (dao/dao.pri)
include (gui/gui.pri)
include (bean/bean.pri)
include (util/util.pri)
include (test/test.pri)

SOURCES += main.cpp
