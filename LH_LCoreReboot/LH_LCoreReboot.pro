TARGET = LH_LCoreReboot
TEMPLATE = lib
DEFINES += LH_LCOREREBOOT_LIBRARY

include(../Plugins.pri)

HEADERS += \
    $$PLUGIN_HEADERS

SOURCES += \
    $$PLUGIN_SOURCES

win32 {
    SOURCES += LH_QtPlugin_LCoreReboot.cpp
    HEADERS += LH_QtPlugin_LCoreReboot.h
    LIBS += -L"C:/Program Files/Microsoft SDKs/Windows/v7.0/Lib" -lpsapi
}
