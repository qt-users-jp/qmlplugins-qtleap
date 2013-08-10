#include "qabstractleappointablemodel.h"

#include <QtCore/QDebug>

#include "qstatistics_p.h"

class QAbstractLeapPointableModel::Private
{
public:
    Private(QAbstractLeapPointableModel *parent);

private:
    QAbstractLeapPointableModel *q;
};

QAbstractLeapPointableModel::Private::Private(QAbstractLeapPointableModel *parent)
    : q(parent)
{
}

QAbstractLeapPointableModel::QAbstractLeapPointableModel(QObject *parent)
    : QAbstractLeapListenerModel(parent)
    , d(new Private(this))
{
}

QHash<int, QByteArray> QAbstractLeapPointableModel::roleNames() const
{
    static QHash<int, QByteArray> ret;
    if (ret.isEmpty()) {
        ret.insert(Qt::UserRole + IdRole, "id");
        ret.insert(Qt::UserRole + TipPositionRole, "tipPosition");
        ret.insert(Qt::UserRole + TipVelocityRole, "tipVelocity");
        ret.insert(Qt::UserRole + DirectionRole, "direction");
        ret.insert(Qt::UserRole + WidthRole, "width");
        ret.insert(Qt::UserRole + LengthRole, "length");
        ret.insert(Qt::UserRole + TouchZoneRole, "touchZone");
        ret.insert(Qt::UserRole + TouchDistanceRole, "touchDistance");
        ret.insert(Qt::UserRole + HandIdRole, "handId");
    }
    return ret;
}

QVariant QAbstractLeapPointableModel::data(const QModelIndex &index, int role) const
{
    QVariant ret;
    switch (role) {
    case Qt::DisplayRole:
        ret = QStringLiteral("Pointable {id:%1}").arg(QAbstractLeapListenerModel::data(index, Qt::UserRole + IdRole).toInt());
        break;
    default:
        ret = QAbstractLeapListenerModel::data(index, role);
        break;
    }

    return ret;
}
