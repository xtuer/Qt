#-------------------------------------------------
#
# Project created by QtCreator 2019-05-30T16:37:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ColourfulDevice
TEMPLATE = app
CONFIG  += c++11
CONFIG  -= app_bundle

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
        ColourfulDeviceWidget.cpp \
        ColourfulDevicesWidget.cpp \
        GradientColorPicker.cpp \
        main.cpp

HEADERS += \
        ColourfulDeviceWidget.h \
        ColourfulDevicesWidget.h \
        GradientColorPicker.h

FORMS += \
    ColourfulDevicesWidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
