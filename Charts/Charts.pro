#-------------------------------------------------
#
# Project created by QtCreator 2018-03-18T16:49:20
#
#-------------------------------------------------

QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = Charts
TEMPLATE = app

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
        Widget.cpp \
    SelectableChartView.cpp \
    RecordCalibrationWidget.cpp \
    GridLine.cpp

HEADERS += \
        Widget.h \
    SelectableChartView.h \
    RecordCalibrationWidget.h \
    GridLine.h

FORMS += \
        Widget.ui \
    RecordCalibrationWidget.ui
