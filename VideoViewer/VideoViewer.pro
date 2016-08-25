#-------------------------------------------------
#
# Project created by QtCreator 2016-08-06T10:51:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VideoViewer
TEMPLATE = app

CONFIG -= app_bundle

# Output directory
CONFIG(debug, debug|release) {
    output = debug
}
CONFIG(release, debug|release) {
    output = release
}

DESTDIR     = bin
OBJECTS_DIR = $$output/obj
MOC_DIR     = $$output/moc
RCC_DIR     = $$output/rcc
UI_DIR      = $$output/ui

SOURCES += main.cpp\
        MainWidget.cpp

HEADERS  += MainWidget.h

FORMS    += MainWidget.ui
