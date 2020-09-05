#-------------------------------------------------
#
# Project created by QtCreator 2020-08-14T22:22:00
#
#-------------------------------------------------

QT       += core gui network networkauth

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION = 1.0.0.7       # 1.0.0.7

CONFIG += c++11

include($$_PRO_FILE_PWD_/src/src.pri)

RC_ICONS = $$_PRO_FILE_PWD_/resource/images/Mozi.ico

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    $$_PRO_FILE_PWD_/resource/rec.qrc
