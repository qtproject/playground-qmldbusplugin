TEMPLATE = lib
TARGET = dbusplugin
QT += quick dbus
CONFIG += qt plugin

DESTDIR = ../qml/com/pelagicore/dbus

HEADERS += \
    dbusplugin.h \
    dbusinterface.h \
    dbusreply.h \
    qdbusinterfacewrapper.h \
    dbus.h

SOURCES += \
    dbusplugin.cpp \
    dbusinterface.cpp \
    dbusreply.cpp \
    qdbusinterfacewrapper.cpp \
    dbus.cpp

OTHER_FILES += \
    ../qml/com/pelagicore/dbus/qmldir \
    dbusplugin.json

qmldir.files = ../qml/com/pelagicore/dbus/qmldir

target.path = $$[QT_INSTALL_QML]/com/pelagicore/dbus
qmldir.path = $$[QT_INSTALL_QML]/com/pelagicore/dbus

INSTALLS+= qmldir target
