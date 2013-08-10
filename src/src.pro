TEMPLATE = subdirs
SUBDIRS = lib

!isEmpty(QT.qml.name) {
    src_imports.subdir = imports
    src_imports.depends = lib
    SUBDIRS += src_imports
}
