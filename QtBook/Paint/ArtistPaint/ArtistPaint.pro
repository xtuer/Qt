#-------------------------------------------------
#
# Project created by QtCreator 2016-12-11T10:09:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ArtistPaint
TEMPLATE = app
CONFIG  -=app_bundle

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

SOURCES += main.cpp\
        PaintButtonWidget.cpp

HEADERS  += PaintButtonWidget.h

FORMS    += PaintButtonWidget.ui
