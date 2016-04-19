#-------------------------------------------------
#
# Project created by QtCreator 2016-04-19T17:58:32
#
#-------------------------------------------------

QT       += core gui sql
CONFIG   += console c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtPHP-SRS
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

Release:DESTDIR = release
Release:OBJECTS_DIR = release/.obj
Release:MOC_DIR = release/.moc
Release:RCC_DIR = release/.rcc
Release:UI_DIR = release/.ui

Debug:DESTDIR = debug
Debug:OBJECTS_DIR = debug/.obj
Debug:MOC_DIR = debug/.moc
Debug:RCC_DIR = debug/.rcc
Debug:UI_DIR = debug/.ui
