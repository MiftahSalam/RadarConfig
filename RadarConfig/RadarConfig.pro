#-------------------------------------------------
#
# Project created by QtCreator 2022-03-24T15:34:15
#
#-------------------------------------------------

QT       -= gui

TARGET = RadarConfig
TEMPLATE = lib
CONFIG += staticlib

DEFINES += RADARCONFIG_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        $$PWD/radarconfig.cpp

HEADERS_BASE += \
        $$PWD/radarconfig.h \
        $$PWD/radarconfig_global.h

HEADERS += $$HEADERS_BASE

unix {
    header.files = $$HEADERS_BASE
    header.path = /usr/include/RadarConfig/2022/v1
    target.path = /usr/lib/RadarConfig/2022/v1/

    INSTALLS += header
    INSTALLS += target
}
