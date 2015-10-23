
#include(/opt/Qt/5.5/Src/qtbase/doc/global/macros.qdocconf) #macros for QDoc commands
#include(/opt/Qt/5.5/Src/qtbase/doc/global/qt-cpp-defines.qdocconf) #needed by C++ projects
#include(/opt/Qt/5.5/Src/qtbase/doc/global/compat.qdocconf) #compatibility macros
#include(/opt/Qt/5.5/Src/qtbase/doc/global/fileextensions.qdocconf) #configuration common among QDoc projects
#include(/opt/Qt/5.5/Src/qtbase/doc/global/qt-html-templates-offline.qdocconf) #offline HTML template for documentation shipped to Qt Creator

QMAKE_TARGET_COMPANY = "Simon Meaden"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2013-2014 Simon Meaden <info@smelecomp.co.uk>"
QMAKE_TARGET_DESCRIPTION = "ExtendedTabWidget, qTabWidget extension for QT5"

CONFIG      += plugin debug_and_release
TARGET      = $$qtLibraryTarget(extendedtabwidgetplugin)
TEMPLATE    = lib

QMAKE_DOCS = $$PWD/extendedtabwidget.qdocconf #the qdocconf file
load(qt_parts)

DEFINES += OSDAB_LIBRARY EXTENDEDTABWIDGET_LIBRARY


CONFIG(debug, debug|release) {
    TARGET = extendedtabwidgetd
    DESTDIR = ../../build/extendedtabwidget
    OBJECTS_DIR = $$DESTDIR/.objd
    MOC_DIR = $$DESTDIR/.mocd
    RCC_DIR = $$DESTDIR/.qrcd
    UI_DIR = $$DESTDIR/.uid
}

CONFIG(release, debug|release) {
    TARGET = extendedtabwidget
    DESTDIR = ../../build/extendedtabwidget
    OBJECTS_DIR = $$DESTDIR/.obj
    MOC_DIR = $$DESTDIR/.moc
    RCC_DIR = $$DESTDIR/.qrc
    UI_DIR = $$DESTDIR/.ui
}

HEADERS     = \
    extendedtabwidgetplugin.h

SOURCES     = \
    extendedtabwidgetplugin.cpp

RESOURCES   = icons.qrc

LIBS        += -L. 

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += designer widgets uiplugin
} else {
    CONFIG += designer
}


target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target


include(extendedtabwidget.pri)

DISTFILES += \
    README.md \
    gpl3.txt \
    extendedtabwidget.qdocconf

