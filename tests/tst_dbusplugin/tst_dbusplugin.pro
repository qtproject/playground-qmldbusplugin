TEMPLATE = app
TARGET = tst_dbusplugin
CONFIG += warn_on qmltestcase
SOURCES += tst_dbusplugin.cpp

OTHER_FILES = *.qml \
    tst_dbusinterface.qml

IMPORTPATH += $$PWD/../../qml $$PWD/../qml
