#-------------------------------------------------
#
# Project created by QtCreator 2016-11-29T07:23:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Carousel
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
        Carousel.cpp \
    CarouselItem.cpp \
    CarouselController.cpp

HEADERS  += Carousel.h \
    CarouselItem.h \
    CarouselController.h
