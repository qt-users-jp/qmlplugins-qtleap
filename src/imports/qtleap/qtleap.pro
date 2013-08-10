IMPORT_VERSION = 0.1
TARGET = leap
QT = qml-private leap
LIBS += -L$$QT.leap.libs

SOURCES += main.cpp

load(qml_plugin)

OTHER_FILES = plugins.qmltypes qmldir
