#-------------------------------------------------
#
# Project created by QtCreator 2016-12-13T13:23:47
#
#-------------------------------------------------

QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RealTimeCurveQChart
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
        RealTimeCurveQChartWidget.cpp

HEADERS  += RealTimeCurveQChartWidget.h

FORMS    +=
