#-------------------------------------------------
#
# Project created by QtCreator 2016-07-18T17:35:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Cleaner
TEMPLATE = app


SOURCES += main.cpp\
        cleaner.cpp

HEADERS  += cleaner.h

FORMS    += cleaner.ui

RC_ICONS = devil.ico
QMAKE_TARGET_COPYRIGHT = Andrey Hudz
VERSION = 1.7.0.0

RESOURCES += \
    cleanerimg.qrc

QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"
