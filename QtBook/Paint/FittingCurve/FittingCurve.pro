#-------------------------------------------------
#
# Project created by QtCreator 2017-10-19T13:31:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = FittingCurve
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

SOURCES += \
        main.cpp \
    FittingCurveWidget.cpp

HEADERS += \
    FittingCurveWidget.h

FORMS +=
