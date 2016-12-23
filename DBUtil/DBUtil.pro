#-------------------------------------------------
#
# Project created by QtCreator 2015-04-14T21:30:37
#
#-------------------------------------------------

QT       += core gui xml sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DBUtil
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

include(util/util.pri)
include(bean/bean.pri)
include(dao/dao.pri)
include(db/db.pri)

SOURCES += main.cpp

HEADERS  +=

FORMS    +=
