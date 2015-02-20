TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

SOURCES += main.cpp \
    figure.cpp \
    square.cpp \
    figurecontainer.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    figure.h \
    square.h \
    figurecontainer.h

