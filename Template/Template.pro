#-------------------------------------------------
#
# Project created by QtCreator 2014-08-27T22:30:14
#
#-------------------------------------------------

QT       += core gui xml sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Template
TEMPLATE = app

CONFIG     -= app_bundle

# 编译输出的文件
DESTDIR     = bin
UI_DIR      = compiled/ui
MOC_DIR     = compiled/moc
OBJECTS_DIR = compiled/obj
RCC_DIR     = compiled/qrc

include (gui/gui.pri)
include (dao/dao.pri)
include (bean/bean.pri)
include (util/util.pri)
include (info/info.pri)
include (test/test.pri)

SOURCES += main.cpp

system("cp dao/UserDao.h $$OUT_PWD/bin/")
