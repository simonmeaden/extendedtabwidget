#-------------------------------------------------
#
# Project created by QtCreator 2015-10-16T11:11:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestExtendedTabWidget
TEMPLATE = app

include(../../extendedtabwidgetfactory/extendedtabwidget.pri)
INCLUDEPATH += ../../extenextendedtabwidgetfactory

CONFIG(debug, debug|release) {
    TARGET = testextendedtabwidgetd
    DESTDIR = ../../build/test/extendedtabwidget
    OBJECTS_DIR = $$DESTDIR/.objd
    MOC_DIR = $$DESTDIR/.mocd
    RCC_DIR = $$DESTDIR/.qrcd
    UI_DIR = $$DESTDIR/.uid
}

CONFIG(release, debug|release) {
    TARGET = testextendedtabwidget
    DESTDIR = ../../build/test/extendedtabwidget
    OBJECTS_DIR = $$DESTDIR/.obj
    MOC_DIR = $$DESTDIR/.moc
    RCC_DIR = $$DESTDIR/.qrc
    UI_DIR = $$DESTDIR/.ui
}

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h
