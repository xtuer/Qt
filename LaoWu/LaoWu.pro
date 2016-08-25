#-------------------------------------------------
#
# Project created by QtCreator 2013-09-23T06:40:23
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LaoWu2
TEMPLATE = app

CONFIG -= app_bundle

DESTDIR     = bin
UI_DIR      = compiled/ui
MOC_DIR     = compiled/moc
OBJECTS_DIR = compiled/obj
RCC_DIR     = compiled/rcc

include(ui/ui.pri)
include(util/util.pri)
include(model/model.pri)
include(domain/domain.pri)
include(sn/sn.pri)

SOURCES += main.cpp
