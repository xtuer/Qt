#-------------------------------------------------
#
# Project created by QtCreator 2016-09-01T12:10:48
#
#-------------------------------------------------

QT       += core gui network xml multimedia multimediawidgets

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

include(lib/lib.pri)
include(magic/magic.pri)
include(gui/gui.pri)
include(bean/bean.pri)
include(dao/dao.pri)
include(util/util.pri)
include(reader/reader.pri)
include(log/log.pri)

include(SingleApplication/singleapplication.pri)
DEFINES += QAPPLICATION_CLASS=QApplication

#LIBS += $$PWD/lib/cardapi3.a
LIBS += $$PWD/lib/cardapi3.dll

SOURCES += main.cpp \
    Constants.cpp

RESOURCES += \
    icon.qrc \
    resources.qrc

HEADERS += \
    Constants.h

