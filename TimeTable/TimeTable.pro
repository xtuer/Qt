#-------------------------------------------------
#
# Project created by QtCreator 2014-08-21T21:49:54
#
#-------------------------------------------------

QT       += core gui sql xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TimeTable
TEMPLATE = app

# Output directory
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

CONFIG     -=app_bundle

include(gui/gui.pri)
include(dao/dao.pri)
include(bean/bean.pri)
include(util/util.pri)
include(test/test.pri)

SOURCES += main.cpp \
    Constants.cpp

HEADERS += \
    Constants.h

OTHER_FILES += \
    app.rc

RC_FILE = \
    app.rc

