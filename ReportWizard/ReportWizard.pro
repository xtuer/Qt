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
        ReportWizardWidget.cpp \
    ReportSettings.cpp \
    Json.cpp

HEADERS += \
        ReportWizardWidget.h \
    ReportSettings.h \
    Json.h

FORMS += \
        ReportWizardWidget.ui
