QT += testlib
QT -= gui

################################################################################

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

################################################################################

TARGET = tst_fdm_lead_lag

################################################################################

DEFINES += QT_DEPRECATED_WARNINGS

################################################################################

INCLUDEPATH += . ./lib_fdm

win32: INCLUDEPATH += \
    $(OSG_ROOT)/include/ \
    $(OSG_ROOT)/include/libxml2

unix: INCLUDEPATH += \
    /usr/include/libxml2

################################################################################

win32: LIBS += \
    -L$(OSG_ROOT)/lib \
    -llibxml2

unix: LIBS += \
    -L/lib \
    -L/usr/lib \
    -lxml2

################################################################################

include(lib_fdm/lib_fdm.pri)

################################################################################

SOURCES += \
    tst_fdm_lead_lag.cpp

################################################################################

DEFINES += SRCDIR=\\\"$$PWD/\\\"
