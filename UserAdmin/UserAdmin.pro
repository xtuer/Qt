#-------------------------------------------------
#
# Project created by QtCreator 2018-05-06T10:40:38
#
#-------------------------------------------------

QT += core gui sql xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = UserAdmin
TEMPLATE = app
CONFIG  -= app_bundle

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

include(gui/gui.pri)
include(db/db.pri)
include(dao/dao.pri)
include(util/util.pri)
include(bean/bean.pri)

SOURCES += \
        main.cpp
