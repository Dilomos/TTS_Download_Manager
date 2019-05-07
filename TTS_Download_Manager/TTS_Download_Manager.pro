#-------------------------------------------------
#
# Project created by QtCreator 2019-04-13T14:26:09
#
#-------------------------------------------------

QT       += core gui 3dcore 3drender 3dinput 3dlogic 3dextras 3danimation

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TTS_Download_Manager
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    orbittransformcontroller.cpp \
    tools.cpp \
    tts_abstractcustomimage.cpp \
    tts_abstractfile.cpp \
    tts_customdeck.cpp \
    tts_customimage.cpp \
    tts_game.cpp \
    tts_custom_model.cpp \
    tts_treewidgetitem.cpp

HEADERS += \
        mainwindow.h \
    orbittransformcontroller.h \
    tools.h \
    tts_abstractcustomimage.h \
    tts_abstractfile.h \
    tts_customdeck.h \
    tts_customimage.h \
    tts_game.h \
    tts_custom_model.h \
    tts_treewidgetitem.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
