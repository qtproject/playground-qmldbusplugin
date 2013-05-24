TEMPLATE = app
TARGET = testserver
QT += dbus
CONFIG += qt

#DESTDIR = ../../qml/testserverplugin

HEADERS += \
    testserver.h \
    testserveradaptor.h

SOURCES += \
    testserver.cpp \
    main.cpp \
    testserveradaptor.cpp
