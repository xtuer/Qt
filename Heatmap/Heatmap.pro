#-------------------------------------------------
#
# Project created by QtCreator 2019-01-13T17:18:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Heatmap
TEMPLATE = app

CONFIG  -= app_bundle
RC_ICONS = AppIcon.ico  # For Win
ICON     = AppIcon.icns # For Mac

DEFINES += QT_MESSAGELOGCONTEXT
DEFINES += QT_DEPRECATED_WARNINGS

# Output directory
CONFIG(debug, debug|release) {
    output = debug
    TARGET = $$TARGET'_d'
}
CONFIG(release, debug|release) {
    output = release
}

DESTDIR     = bin
OBJECTS_DIR = $$output
MOC_DIR     = $$output
RCC_DIR     = $$output
UI_DIR      = $$output

include(heatmap/heatmap.pri)

SOURCES += main.cpp

