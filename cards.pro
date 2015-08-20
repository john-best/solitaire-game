#-------------------------------------------------
#
# Project created by QtCreator 2015-07-15T14:55:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cards
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    card.cpp \
    pile.cpp \
    game.cpp \
    klondike.cpp \
    rule.cpp \
    move.cpp \
    windialog.cpp \
    helpdialog.cpp \
    spider.cpp \
    freecell.cpp \
    pyramid.cpp

HEADERS  += mainwindow.h \
    card.h \
    pile.h \
    game.h \
    klondike.h \
    rule.h \
    move.h \
    windialog.h \
    helpdialog.h \
    spider.h \
    freecell.h \
    pyramid.h

FORMS    += mainwindow.ui \
    windialog.ui \
    helpdialog.ui

RESOURCES += \
    cards.qrc
