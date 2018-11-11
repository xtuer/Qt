#-------------------------------------------------
#
# Project created by QtCreator 2018-06-10T18:39:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ReportWizard
TEMPLATE = app

# Output directory
CONFIG(debug, debug|release) {
    output = debug
    TARGET = ReportWizard_d
}
CONFIG(release, debug|release) {
    output = release
}

DESTDIR     = bin
OBJECTS_DIR = $$output
MOC_DIR     = $$output
RCC_DIR     = $$output
UI_DIR      = $$output

include(bean/bean.pri)
include(gui/gui.pri)
include(gui-report/gui-report.pri)
include(util/util.pri)

SOURCES += \
        main.cpp

