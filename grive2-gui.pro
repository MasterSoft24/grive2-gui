
#-------------------------------------------------
#
# Project created by QtCreator 2014-09-28T21:58:40
#
#-------------------------------------------------

QT       += core gui network webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = grive2-gui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    addsyncwizard.cpp \
    tabs/tabSyncData.cpp \
    mqwebview.cpp \
    mqwebpage.cpp \
    tabs/driveFileList.cpp

HEADERS  += mainwindow.h \
    addsyncwizard.h \
    tabs/tabSyncData.h \
    mqwebview.h \
    mqwebpage.h

FORMS    += mainwindow.ui \
    AddSyncWizard.ui

RESOURCES += \
    res.qrc

OTHER_FILES += \
    COPYING
