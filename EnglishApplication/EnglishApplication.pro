QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addqdiolog.cpp \
    addwordswidget.cpp \
    database.cpp \
    editdialog.cpp \
    exercisewidget.cpp \
    learnwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    mycentralwidget.cpp

HEADERS += \
    addqdiolog.h \
    addwordswidget.h \
    database.h \
    editdialog.h \
    exercisewidget.h \
    learnwidget.h \
    mainwindow.h \
    mycentralwidget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
