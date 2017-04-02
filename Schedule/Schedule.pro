#-------------------------------------------------
#
# Project created by QtCreator 2016-08-20T15:00:56
#
#-------------------------------------------------

QT       += core gui sql xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG  += c++11
CONFIG  -=app_bundle

TARGET   = Schedule
TEMPLATE = app
RC_ICONS = AppIcon.ico

DEFINES += QT_DEPRECATED_WARNINGS

# Output directory
CONFIG(debug, debug|release) {
    compiled = debug
}
CONFIG(release, debug|release) {
    compiled = release
}

DESTDIR     = bin
OBJECTS_DIR = $$compiled
MOC_DIR     = $$compiled
RCC_DIR     = $$compiled
UI_DIR      = $$compiled

include(bean/bean.pri)
include(dao/dao.pri)
include(db/db.pri)
include(gui/gui.pri)
include(util/util.pri)
include(test/test.pri)
include(MagicWindow/MagicWindow.pri)

# Copy promotion required headers to build directory
win32 {
    COPY_DEST = $$replace(OUT_PWD, /, \\)
    system("copy gui\\ClassWidget.h   $$COPY_DEST\\$$compiled\\ClassWidget.h")
    system("copy gui\\CourseWidget.h  $$COPY_DEST\\$$compiled\\CourseWidget.h")
    system("copy gui\\TeacherWidget.h $$COPY_DEST\\$$compiled\\TeacherWidget.h")
}

mac {
    system("cp gui/ClassWidget.h   $$OUT_PWD/$$compiled/ClassWidget.h")
    system("cp gui/CourseWidget.h  $$OUT_PWD/$$compiled/CourseWidget.h")
    system("cp gui/TeacherWidget.h $$OUT_PWD/$$compiled/TeacherWidget.h")
}

SOURCES += main.cpp \
    Constants.cpp

RESOURCES += \
    resources.qrc

HEADERS += \
    Constants.h

