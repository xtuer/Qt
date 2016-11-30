#-------------------------------------------------
#
# Project created by QtCreator 2016-12-01T07:32:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CarouselTheory
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

SOURCES += main.cpp \
    Carousel.cpp

HEADERS  += \
    Carousel.h
