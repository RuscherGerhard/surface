#-------------------------------------------------
#
# Project created by QtCreator 2019-10-09T19:21:09
#
#-------------------------------------------------

QT       += core gui
QT      +=  charts

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
        btnresultswitch.cpp \
        filters/boxfilter.cpp \
        filters/filter.cpp \
        filters/gaussfilter.cpp \
        filters/gaussnl.cpp \
        filters/input.cpp \
        filters/linefindertransversal.cpp \
        filters/output.cpp \
        filters/probaddscrambler.cpp \
        filters/segmentator.cpp \
        filters/shapefollower.cpp \
        filters/skeletonizer.cpp \
        filters/turtleshapefollower.cpp \
        histogram.cpp \
        logger.cpp \
        logview.cpp \
        main.cpp \
        mainifc.cpp \
        mainwindow.cpp \
        myspecialpixmapitem.cpp \
        pipe.cpp \
        pipeconfig.cpp \
        pipemanager.cpp \
        utils.cpp

HEADERS += \
        btnresultswitch.h \
        filters/boxfilter.h \
        filters/filter.h \
        filters/filterutils.h \
        filters/gaussfilter.h \
        filters/gaussnl.h \
        filters/input.h \
        filters/linefindertransversal.h \
        filters/output.h \
        filters/probaddscrambler.h \
        filters/segmentator.h \
        filters/shapefollower.h \
        filters/skeletonizer.h \
        filters/turtleshapefollower.h \
        histogram.h \
        logger.h \
        logview.h \
        mainifc.h \
        mainwindow.h \
        myspecialpixmapitem.h \
        pipe.h \
        pipeconfig.h \
        pipemanager.h \
        utils.h

FORMS += \
        logview.ui \
        mainwindow.ui \
        pipeconfig.ui

LIBS += -ltinyxml2

LIBS += /usr/lib/x86_64-linux-gnu/libtinyxml2.so

#OpenCV Libs
LIBS += /usr/local/lib/libopencv_core.so
LIBS += /usr/local/lib/libopencv_imgproc.so
LIBS += /usr/local/lib/libopencv_imgcodecs.so

INCLUDEPATH += /usr/include/tinyxml2/tinysml2.h

#opencv inlcude
INCLUDEPATH += /usr/local/include/opencv4


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
