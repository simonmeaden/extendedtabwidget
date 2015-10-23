
INCLUDEPATH += $$PWD/src

include(osdab/base64.pri)

HEADERS += \
    $$PWD/ExtendedTabWidget \
    $$PWD/extendedtabwidget_global.h \
    $$PWD/extendedtabwidget.h \
    $$PWD/extendedtabwidget_p.h

SOURCES += \
    $$PWD/extendedtabwidget.cpp \
    $$PWD/extendedtabwidget_p.cpp
