#-------------------------------------------------
#
# Project created by QtCreator 2014-11-24T10:35:35
#
#-------------------------------------------------

QT       += core gui opengl network

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NetworkTanksFirstMovingClient
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    Scene.cpp \
    ConnectionDialog.cpp \
    Client.cpp \
    Tank.cpp

HEADERS  += MainWindow.h \
    Scene.h \
    ConnectionDialog.h \
    Client.h \
    Tank.h

FORMS    += MainWindow.ui \
    ConnectionDialog.ui

RESOURCES += \
    Textures.qrc
