#-------------------------------------------------
#
# Project created by QtCreator 2016-12-10T12:41:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Base
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
        MainWidget.cpp \
    GridWidget.cpp \
    MultipleLinesWidget.cpp \
    PolylineWidget.cpp \
    PolygonWidget.cpp \
    PolygonCircleWidget.cpp \
    RoundRectWidget.cpp \
    EllipseWidget.cpp \
    ArcChordPieWidget.cpp

HEADERS  += MainWidget.h \
    GridWidget.h \
    MultipleLinesWidget.h \
    PolylineWidget.h \
    PolygonWidget.h \
    PolygonCircleWidget.h \
    RoundRectWidget.h \
    EllipseWidget.h \
    ArcChordPieWidget.h

FORMS    += MainWidget.ui \
    GridWidget.ui \
    MultipleLinesWidget.ui \
    PolylineWidget.ui \
    PolygonWidget.ui \
    PolygonCircleWidget.ui \
    RoundRectWidget.ui \
    EllipseWidget.ui \
    ArcChordPieWidget.ui

