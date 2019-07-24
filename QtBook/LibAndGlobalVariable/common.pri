CONFIG  += c++11

DEFINES += QT_MESSAGELOGCONTEXT
DEFINES += QT_DEPRECATED_WARNINGS

# 工程根目录，子项目里头文件可以使用 #include <MyLibrary/xxx.h> 的方式包含
project      = $$PWD
INCLUDEPATH += $$project

# 编译输出目录
CONFIG(debug, debug|release) {
    bin    = $$OUT_PWD/../bin-debug
    output = $$OUT_PWD/debug
}
CONFIG(release, debug|release) {
    bin    = $$OUT_PWD/../bin-release
    output = $$OUT_PWD/release
}

DESTDIR     = $$bin
MOC_DIR     = $$output
RCC_DIR     = $$output
UI_DIR      = $$output
OBJECTS_DIR = $$output
