#-------------------------------------------------
#
# Project created by QtCreator 2012-02-06T11:51:14
#
#-------------------------------------------------

QT       += core gui

TARGET = CubGL
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    glwidget.cpp \
    cub.cpp \
    Common.cpp

HEADERS  += \
    mainwindow.h \
    glwidget.h \
    cub.h \
    Common.h \
    vec.h \
    mat.h

QT           += opengl
QT           += widgets
# install

sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS CubGL.pro
INSTALLS +=  sources

OTHER_FILES += \
    README.txt





























