#-------------------------------------------------
#
# Project created by QtCreator 2012-02-07T16:04:09
#
#-------------------------------------------------

QT       += core gui opengl
QT       += widgets

TARGET = BillarPractica2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    Common.cpp \
    escena.cpp \
    objecte.cpp \
    taulabillar.cpp \
    cara.cpp \
    readfile.cpp \
    plabase.cpp \
    bola.cpp \
    conjuntboles.cpp \
    camera.cpp \
    llum.cpp \
    conjuntllums.cpp \
    material.cpp

HEADERS  += mainwindow.h \
    vec.h \
    mat.h \
    glwidget.h \
    Common.h \
    escena.h \
    objecte.h \
    taulabillar.h \
    cara.h \
    readfile.h \
    plabase.h \
    bola.h \
    conjuntboles.h \
    camera.h \
    llum.h \
    conjuntllums.h \
    material.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    vshaderFlatGouraud.glsl \
    fshaderFlatGouraud.glsl \
    vshaderPhong.glsl \
    fshaderPhong.glsl \
    vshaderToon.glsl \
    fshaderToon.glsl

RESOURCES += \
    resources.qrc







