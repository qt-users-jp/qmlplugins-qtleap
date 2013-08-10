#include "qabstractleapmodel.h"

#include <QtCore/QDebug>
#include <QtCore/QMetaType>
#include <QtCore/QTimer>
#include <QtGui/QVector3D>

class QAbstractLeapModel::Private
{
public:
    Private(QAbstractLeapModel *parent);

private:
    QAbstractLeapModel *q;

public:
    bool enabled;
    bool connected;
    QHash<int, QVariantList> data;
    QList<int> id;
    QList<int> idToBeRemoved;
    Leap::Controller controller;
    QTimer timer;
};

QAbstractLeapModel::Private::Private(QAbstractLeapModel *parent)
    : q(parent)
    , enabled(false)
    , connected(false)
{
    if (connected != controller.isConnected()) {
        connected = controller.isConnected();
        emit q->connectedChanged(connected);
    }
    timer.setInterval(1000 / 15);
    connect(&timer, &QTimer::timeout, [this]() {
        if (connected != controller.isConnected()) {
            connected = controller.isConnected();
            emit q->connectedChanged(connected);
        }
        if (connected) {
            q->timeout(controller);
//            q->onFrame(controller);
        }
    });
}

QAbstractLeapModel::QAbstractLeapModel(QObject *parent)
    : QAbstractListModel(parent)
    , d(new Private(this))
{
    qRegisterMetaType<QVector<int>>("QVector<int>");
    connect(this, &QAbstractLeapModel::destroyed, [this]() { delete d; });
}

int QAbstractLeapModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return d->id.length();
}

QVariant QAbstractLeapModel::data(const QModelIndex &index, int role) const
{
//    qDebug() << Q_FUNC_INFO << __LINE__ << index.row() << role;
    QVariant ret;
    if (roleNames().contains(role)) {
        ret = d->data.value(d->id.at(index.row())).value(role - Qt::UserRole);
    } else if (role >= Qt::UserRole) {
        qDebug() << Q_FUNC_INFO << __LINE__ << role - Qt::UserRole << d->data.value(d->id.at(index.row()));
    }
    return ret;
}

Leap::Controller &QAbstractLeapModel::controller() const
{
    return d->controller;
}

QVariantMap QAbstractLeapModel::get(int row) const
{
    QVariantMap ret;
    QVariantList data = d->data.value(d->id.at(row));
    foreach (int role, roleNames().keys()) {
        ret.insert(roleNames().value(role), data.value(role - Qt::UserRole));
    }
    return ret;
}

void QAbstractLeapModel::beginUpdate()
{
    d->idToBeRemoved = d->id;
}

void QAbstractLeapModel::upsert(int id, const QVariantList &data)
{
//    qDebug() << Q_FUNC_INFO << __LINE__ << d->id << d->data;
    if (d->idToBeRemoved.contains(id)) {
        d->idToBeRemoved.removeOne(id);
        int row = d->id.indexOf(id);
        QVariantList oldData = d->data.take(id);
        QVector<int> roles;
        foreach (int role, roleNames().keys()) {
            if (oldData.value(role - Qt::UserRole) != data.value(role - Qt::UserRole)) {
                roles.append(role);
            }
        }

        d->data.insert(id, data);
        QModelIndex i = index(row, 0);
        emit dataChanged(i, i, roles);
    } else {
        beginInsertRows(
                    QModelIndex()
                    , d->id.length()
                    , d->id.length()
                    );
        d->id.append(id);
        d->data.insert(id, data);
        endInsertRows();
        emit countChanged(rowCount());
    }
//    qDebug() << Q_FUNC_INFO << __LINE__ << d->id << d->data;
}

void QAbstractLeapModel::endUpdate()
{
    if (d->idToBeRemoved.isEmpty()) return;
//    qDebug() << Q_FUNC_INFO << __LINE__ << d->idToBeRemoved;
//    qDebug() << Q_FUNC_INFO << __LINE__ << d->id << d->data.keys();
    foreach (int id, d->idToBeRemoved) {
        int row = d->id.indexOf(id);
        beginRemoveRows(QModelIndex(), row, row);
        d->id.removeAt(row);
        d->data.remove(id);
        endRemoveRows();
    }
    emit countChanged(rowCount());
//    qDebug() << Q_FUNC_INFO << __LINE__ << d->id << d->data.keys();
}

int QAbstractLeapModel::count() const
{
    return rowCount();
}

bool QAbstractLeapModel::isConnected() const
{
    return d->connected;
}

bool QAbstractLeapModel::isEnabled() const
{
    return d->enabled;
}

void QAbstractLeapModel::setEnabled(bool enabled)
{
    if (d->enabled == enabled) return;
    d->enabled = enabled;
    if (enabled) {
        d->timer.start();
    } else {
        d->timer.stop();
        beginUpdate();
        endUpdate();
    }
    emit enabledChanged(enabled);
}
