QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG  += c++17
CONFIG  -= app_bundle
TARGET   = PCB-4.0

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CentralWidget.cpp \
    SlotHistory.cpp \
    SlotHome.cpp \
    SlotNavigator.cpp \
    SlotParams.cpp \
    SlotWarning.cpp \
    UiUtil.cpp \
    main.cpp

HEADERS += \
    CentralWidget.h \
    SlotHistory.h \
    SlotHome.h \
    SlotNavigator.h \
    SlotParams.h \
    SlotWarning.h \
    UiUtil.h

FORMS += \
    CentralWidget.ui \
    SlotHistory.ui \
    SlotHome.ui \
    SlotNavigator.ui \
    SlotParams.ui \
    SlotWarning.ui

# Output directory
CONFIG(debug, debug|release) {
    output = debug
    TARGET = PCB-4.0_d
}
CONFIG(release, debug|release) {
    output = release
}

DESTDIR     = bin
OBJECTS_DIR = $$output
MOC_DIR     = $$output
RCC_DIR     = $$output
UI_DIR      = $$output

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
