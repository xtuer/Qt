#-------------------------------------------------
#
# Project created by QtCreator 2016-06-30T17:44:13
#
#-------------------------------------------------

QT       += core gui network xmlpatterns charts webenginewidgets webengine sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET   = Gui
TEMPLATE = app
ICON     = AppIcon.icns
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
    User.cpp \
    Form.cpp \
    SmoothCurveGenerator.cpp \
    NewForm.cpp \
    DraggableLabel.cpp

FORMS += \
    Form.ui \
    NewForm.ui

HEADERS += \
    User.h \
    Form.h \
    SmoothCurveGenerator.h \
    NewForm.h \
    DraggableLabel.h
