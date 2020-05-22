#-------------------------------------------------
#
# Project created by QtCreator 2019-10-09T19:21:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Surface
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        filters/boxfilter.cpp \
        filters/gaussfilter.cpp \
        filters/input.cpp \
        filters/output.cpp \
        main.cpp \
        mainifc.cpp \
        mainwindow.cpp \
        pipe.cpp \
        pipeconfig.cpp \
        pipemanager.cpp \
        utils.cpp

HEADERS += \
        filters/boxfilter.h \
        filters/filter.h \
        filters/gaussfilter.h \
        filters/input.h \
        filters/output.h \
        mainifc.h \
        mainwindow.h \
        pipe.h \
        pipeconfig.h \
        pipemanager.h \
        utils.h

FORMS += \
        mainwindow.ui \
        pipeconfig.ui

LIBS += -ltinyxml2

LIBS += /usr/lib/x86_64-linux-gnu/libtinyxml2.so

INCLUDEPATH += /usr/include/tinyxml2/tinysml2.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
