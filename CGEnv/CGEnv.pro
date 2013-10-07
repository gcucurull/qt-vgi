#-------------------------------------------------
#
# Project created by QtCreator 2012-04-15T17:37:59
#
#-------------------------------------------------

QT       += core gui opengl

win32: LIBS += "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Lib\glew32.lib"

macx: LIBS += -framework GLUT

# només Linux
unix:!macx: LIBS += -lGLEW -lGLU -lGL -lglut

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

macx:  APP_QML_FILES.files = simple.vert simple.frag
macx:  APP_QML_FILES.path = Contents/MacOS
macx:  QMAKE_BUNDLE_DATA += APP_QML_FILES
