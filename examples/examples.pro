TEMPLATE = subdirs
!isEmpty(QT.leap.name) {
	!isEmpty(QT.widgets.name): SUBDIRS += widget
	!isEmpty(QT.quick.name): SUBDIRS += quick
	!isEmpty(QT.3dquick.name): SUBDIRS += quick3d
}
