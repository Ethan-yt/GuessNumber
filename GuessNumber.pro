#-------------------------------------------------
#
# Project created by QtCreator 2015-04-28T16:37:40
#
#-------------------------------------------------

QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GuessNumber
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    guessnumber.cpp \
    dialog.cpp

HEADERS  += mainwindow.h \
    guessnumber.h \
    dialog.h

FORMS    += mainwindow.ui \
    dialog.ui

OTHER_FILES +=

DISTFILES +=

RESOURCES += \
    style.qrc
