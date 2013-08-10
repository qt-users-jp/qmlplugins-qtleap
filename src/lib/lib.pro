TARGET = QtLeap
MODULE = leap
QT = core gui

load(qt_module)

PUBLIC_HEADERS =
HEADERS = $$PUBLIC_HEADERS \
    qleaphandmodel.h \
    qabstractleapmodel.h \
    qtleap_global.h \
    qstatistics_p.h \
    qleapfingermodel.h \
    qleappointablemodel.h \
    qabstractleappointablemodel.h \
    qleaptoolmodel.h \
    qabstractleaplistenermodel.h \
    qleapgesturemodel.h
SOURCES = \
    qleaphandmodel.cpp \
    qabstractleapmodel.cpp \
    qleapfingermodel.cpp \
    qleappointablemodel.cpp \
    qabstractleappointablemodel.cpp \
    qleaptoolmodel.cpp \
    qabstractleaplistenermodel.cpp \
    qleapgesturemodel.cpp

DEFINES += QTLEAP_LIBRARY

LIBS += -lLeap
