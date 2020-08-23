INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/mainwidget.cpp

HEADERS += \
    $$PWD/mainwidget.h

FORMS += \
    $$PWD/mainwidget.ui

include($$PWD/network/network.pri)
include($$PWD/about/about.pri)
include($$PWD/customWidget/customWidget.pri)
