TARGET = LH_Lg320x240
TEMPLATE = lib
DEFINES += LG320x240_LIBRARY

include(../Plugins.pri)
include(../USB.pri)

SOURCES += \
    $$PLUGIN_SOURCES \
    ../LH_QtDevice.cpp \
    ../wow64.cpp \
    LH_Lg320x240.cpp \
    Lg320x240Device.cpp \
    LogitechG19Thread.cpp \
    LogitechG19.cpp

HEADERS += \
    $$PLUGIN_HEADERS \
    ../LH_QtDevice.h \
    ../wow64.h \
    LH_Lg320x240.h \
    Lg320x240Device.h \
    LogitechG19Thread.h \
    LogitechG19.h
