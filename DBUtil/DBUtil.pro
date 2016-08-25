#-------------------------------------------------
#
# Project created by QtCreator 2015-04-14T21:30:37
#
#-------------------------------------------------

QT       += core gui xml sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DBUtil
TEMPLATE = app

CONFIG     -= app_bundle
CONFIG     += c++11

# 编译输出的文件
DESTDIR     = bin
UI_DIR      = compiled/ui
MOC_DIR     = compiled/moc
OBJECTS_DIR = compiled/obj
RCC_DIR     = compiled/qrc

include(util/util.pri)
include(bean/bean.pri)
include(dao/dao.pri)
include(db/db.pri)

SOURCES += main.cpp

HEADERS  +=

FORMS    +=
