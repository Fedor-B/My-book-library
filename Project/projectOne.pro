QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    authorization.cpp \
    checkin.cpp \
    database.cpp \
    databaseprivate.cpp \
    dialogbook.cpp \
    dialogbookprivate.cpp \
    guestclass.cpp \
    main.cpp \
    mainwindow.cpp \
    onlyview.cpp \
    onlyviewprivate.cpp \
    privatewindow.cpp

HEADERS += \
    Crypter.h \
    authorization.h \
    checkin.h \
    database.h \
    databaseprivate.h \
    dialogbook.h \
    dialogbookprivate.h \
    guestclass.h \
    mainwindow.h \
    onlyview.h \
    onlyviewprivate.h \
    privatewindow.h

FORMS += \
    authorization.ui \
    checkin.ui \
    dialogbook.ui \
    dialogbookprivate.ui \
    guestclass.ui \
    mainwindow.ui \
    onlyview.ui \
    onlyviewprivate.ui \
    privatewindow.ui

TARGET = DataBase
TEMPLATE = app

