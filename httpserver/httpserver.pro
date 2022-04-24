# Created by and for Qt Creator This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

#TARGET = httpserver
DEFINES += CPPHTTPLIB_OPENSSL_SUPPORT

QT = core gui widgets

HEADERS = \
   $$PWD/httplib.h

SOURCES = \
   $$PWD/httpserver.cpp

INCLUDEPATH = \
    $$PWD/.

#DEFINES = 

DISTFILES += \
    Makefile

