#-------------------------------------------------
#
# Project created by QtCreator 2011-05-23T14:52:35
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32 {
    RC_FILE = Res.rc
    QT += winextras # requires Qt5
}

TARGET = simpletimer
TEMPLATE = app

SOURCES += Main.cpp \
    MainFrm.cpp

HEADERS  += MainFrm.h

FORMS    += MainFrm.ui
