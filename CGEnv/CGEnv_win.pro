#-------------------------------------------------
#
# Project created by QtCreator 2012-04-15T17:37:59
#
#-------------------------------------------------

QT       += core gui opengl

# Llibreria glew 32 bits (posar dll's 32 a debug i Release)
win32: LIBS += "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Lib\glew32.lib"
#win32: LIBS += "lib\glew32.lib"

# Llibreria glew 64 bits (posar dll's 32 a debug i Release)
#win64: LIBS += "C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib\glew32.lib"

macx: LIBS += -framework GLUT

TARGET = CGEnv
TEMPLATE = app


SOURCES += main.cpp\
        cgenvwindow.cpp \
    GLWidget.cpp \
    visualitzacio.cpp \
    Model.cpp \
    objLoader.cpp \
    Load3DS.cpp \
    Obj3DS.cpp

HEADERS  += cgenvwindow.h \
    GLWidget.h \
    visualitzacio.h \
    Util.h \
    Model.h \
    objLoader.h \
    Load3DS.h \
    Obj3DS.h

FORMS    += cgenvwindow.ui

OTHER_FILES += \
    simple.vert \
    simple.frag
