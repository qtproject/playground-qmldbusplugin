TEMPLATE = subdirs

SUBDIRS += src \
           tests

exists($(QTDIR)/bin/qdoc) {
    exists($(QTDIR)/bin/qhelpgenerator) {
        message ("Using qdoc/qhelpgenerator in QTDIR for generating docs")
        QDOC = $(QTDIR)/bin/qdoc
        QHELPGENERATOR = $(QTDIR)/bin/qhelpgenerator
    } else {
        message ("Trying to use system qdoc/qhelpgenerator for generating docs")
        QDOC = qdoc3
        QHELPGENERATOR = qhelpgenerator
    }
} else {
    message ("Trying to use system qdoc/qhelpgenerator for generating docs")
    QDOC = qdoc3
    QHELPGENERATOR = qhelpgenerator
}

html-docs.commands = $$QDOC $$PWD/doc/qmldbusplugin.qdocconf
html-docs.files = $$PWD/doc/html

qch-docs.commands = $$QHELPGENERATOR $$PWD/doc/html/qmldbusplugin.qhp -o $$PWD/doc/html/qmldbusplugin.qch
qch-docs.files = $$PWD/docs/qch
qch-docs.CONFIG += no_check_exist directory

docs.depends = html-docs qch-docs

QMAKE_EXTRA_TARGETS += html-docs qch-docs docs

OTHER_FILES += doc/qmldbusplugin.qdocconf \
               doc/index.qdoc \
               doc/elements.qdoc
