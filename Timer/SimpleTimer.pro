#-------------------------------------------------
#
# Project created by QtCreator 2011-05-23T14:52:35
#
#-------------------------------------------------

QT       += core gui

TARGET = simpletimer
TEMPLATE = app


SOURCES += Main.cpp\
    MainFrm.cpp

HEADERS  += \
    MainFrm.h \
    WinUtils.h

FORMS    += \
    MainFrm.ui

RESOURCES += \
    Resources.qrc

win32 {
    RC_FILE = Res.rc
    LIBS +=  libole32
}
