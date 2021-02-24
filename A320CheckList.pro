QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

RC_ICONS = ./icons/win_icon.ico

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
    src/checklistframe/checklistbottomtoolbar.cpp \
    src/checklistframe/checklistframe.cpp \
    src/checklistframe/checklistitem.cpp \
    src/configuration.cpp \
    src/datablock.cpp \
    src/datasettingsbottomtoolbar.cpp \
    src/datasettingsframe.cpp \
    src/main.cpp \
    src/maintoolbar.cpp \
    src/mainwindow.cpp

HEADERS += \
    src/SimConnect.h \
    src/checklistframe/checklistbottomtoolbar.h \
    src/checklistframe/checklistdefine.h \
    src/checklistframe/checklistframe.h \
    src/checklistframe/checklistitem.h \
    src/configuration.h \
    src/datablock.h \
    src/datasettingsbottomtoolbar.h \
    src/datasettingsframe.h \
    src/globalvars.h \
    src/maintoolbar.h \
    src/mainwindow.h \
    src/mxmlparser.h \
    src/simdatacollector.h

LIBS += -L./. -lmxmlparser
LIBS += -L./. -lSimConnect
LIBS += -L./. -lSimDataCollector

LIBS += \
    user32.lib \
    Gdi32.lib

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
