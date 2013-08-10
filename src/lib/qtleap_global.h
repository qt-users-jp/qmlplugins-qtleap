#ifndef QTLEAP_GLOBAL_H
#define QTLEAP_GLOBAL_H

#ifdef QTLEAP_LIBRARY
# define QTLEAP_EXPORT Q_DECL_EXPORT
#else
# define QTLEAP_EXPORT Q_DECL_IMPORT
#endif

#include <Leap.h>
#include <QtCore/QDebug>
#include <QtGui/QVector3D>

inline QVector3D l2q(Leap::Vector v) { return QVector3D(v.x, v.y, v.z); }
inline Leap::Vector q2l(QVector3D v) { return Leap::Vector(v.x(), v.y(), v.z()); }

#endif // QTLEAP_GLOBAL_H
