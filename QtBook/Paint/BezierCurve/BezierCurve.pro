#-------------------------------------------------
#
# Project created by QtCreator 2016-12-10T19:30:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BezierCurve
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
        BezierCurveWidget.cpp

HEADERS  += BezierCurveWidget.h

FORMS    += BezierCurveWidget.ui
