#-------------------------------------------------
#
# Project created by QtCreator 2015-10-14T09:43:05
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += winextras

TARGET = BB_threds
TEMPLATE = app

win32:RC_FILE = bb.rc

SOURCES += main.cpp \
    boredombreaker.cpp \
    hid.c \
    dongle.cpp \
    l2window.cpp \
    xpbar.cpp \
    keycondition.cpp \
    keysettingsdialog.cpp \
    keyconditionsset.cpp \
    systemkeyboardreadwrite.cpp \
    l2parser.cpp \
    clicker.cpp \
    dongleworker.cpp \
    systemmousehook.cpp

HEADERS  += \
    boredombreaker.h \
    hidapi.h \
    dongle.h \
    l2window.h \
    xpbar.h \
    keycondition.h \
    keysettingsdialog.h \
    keyconditionsset.h \
    systemkeyboardreadwrite.h \
    l2parser.h \
    clicker.h \
    dongleworker.h \
    systemmousehook.h

FORMS    += \
    boredombreaker.ui \
    keysettingsdialog.ui \
    clicker.ui

LIBS += -lhid
LIBS += -lsetupapi
LIBS += -lgdi32
LIBS += -luser32

