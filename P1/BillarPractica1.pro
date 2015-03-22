#-------------------------------------------------
#
# Project created by QtCreator 2012-02-07T16:04:09
#
#-------------------------------------------------

QT       += core gui opengl
QT       += widgets

TARGET = BillarPractica1
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
    conjuntboles.cpp

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
    conjuntboles.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    vshader1.glsl \
    fshader1.glsl

RESOURCES += \
    resources.qrc

macx {
    copyfiles.commands = cp -r $${PWD}/resources ./../build-BillarPractica1-Desktop_Qt_5_4_1_clang_64bit-Debug/BillarPractica1.app/Contents
    copyfiles.commands = cp -r $${PWD}/*.glsl ./../build-BillarPractica1-Desktop_Qt_5_4_1_clang_64bit-Debug/BillarPractica1.app/Contents
}

QMAKE_EXTRA_TARGETS += copyfiles
POST_TARGETDEPS += copyfiles







