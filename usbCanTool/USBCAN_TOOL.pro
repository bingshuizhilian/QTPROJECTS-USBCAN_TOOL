#-------------------------------------------------
#
# Project created by QtCreator 2018-09-01T15:02:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = USBCAN_TOOL
TEMPLATE = app
RC_ICONS = ../resources/appicon.ico

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp\
        mainwindow.cpp \
    configusbcan.cpp

HEADERS  += mainwindow.h \
    configusbcan.h

FORMS    += mainwindow.ui \
    configusbcan.ui

win32: LIBS += -L$$PWD/libusb-1.0.22/MinGW32/static/ -llibusb-1.0

INCLUDEPATH += $$PWD/libusb-1.0.22/MinGW32/static\
            $$PWD/libusb-1.0.22/include/libusb-1.0
DEPENDPATH += $$PWD/libusb-1.0.22/MinGW32/static

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libusb-1.0.22/MinGW32/static/libusb-1.0.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/libusb-1.0.22/MinGW32/static/libusb-1.0.a
