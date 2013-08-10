#include "qleaphandmodel.h"

#include <QtCore/QDebug>

#include "qstatistics_p.h"

class QLeapHandModel::Private
{
public:
    enum Role {
        IdRole = 0
        , PalmPositionRole
        , PalmVelocityRole
        , PalmNormalRole
        , DirectionRole
        , SphereCenterRole
        , SphereRadiusRole
    };

    Private(QLeapHandModel *parent);

private:
    QLeapHandModel *q;
};

QLeapHandModel::Private::Private(QLeapHandModel *parent)
    : q(parent)
{
}

QLeapHandModel::QLeapHandModel(QObject *parent)
    : QAbstractLeapListenerModel(parent)
    , d(new Private(this))
{
}

QHash<int, QByteArray> QLeapHandModel::roleNames() const
{
    static QHash<int, QByteArray> ret;
    if (ret.isEmpty()) {
        ret.insert(Qt::UserRole + Private::IdRole, "id");
        ret.insert(Qt::UserRole + Private::PalmPositionRole, "palmPosition");
        ret.insert(Qt::UserRole + Private::PalmVelocityRole, "palmVelocity");
        ret.insert(Qt::UserRole + Private::PalmNormalRole, "palmNormal");
        ret.insert(Qt::UserRole + Private::DirectionRole, "direction");
        ret.insert(Qt::UserRole + Private::SphereCenterRole, "sphereCenter");
        ret.insert(Qt::UserRole + Private::SphereRadiusRole, "sphereRadius");
    }
    return ret;
}

QVariant QLeapHandModel::data(const QModelIndex &index, int role) const
{
    QVariant ret;
    switch (role) {
    case Qt::DisplayRole:
        ret = QStringLiteral("Hand {id:%1}").arg(QAbstractLeapListenerModel::data(index, Qt::UserRole + Private::IdRole).toInt());
        break;
    default:
        ret = QAbstractLeapListenerModel::data(index, role);
        break;
    }

    return ret;
}

void QLeapHandModel::onFrame(const Leap::Controller &controller)
{

    QMap<int, int> ids;
    QMap<int, QStatistics<float>> palmPositionX;
    QMap<int, QStatistics<float>> palmPositionY;
    QMap<int, QStatistics<float>> palmPositionZ;
    QMap<int, QStatistics<float>> palmVelocityX;
    QMap<int, QStatistics<float>> palmVelocityY;
    QMap<int, QStatistics<float>> palmVelocityZ;
    QMap<int, QStatistics<float>> palmNormalX;
    QMap<int, QStatistics<float>> palmNormalY;
    QMap<int, QStatistics<float>> palmNormalZ;
    QMap<int, QStatistics<float>> directionX;
    QMap<int, QStatistics<float>> directionY;
    QMap<int, QStatistics<float>> directionZ;
    QMap<int, QStatistics<float>> sphereCenterX;
    QMap<int, QStatistics<float>> sphereCenterY;
    QMap<int, QStatistics<float>> sphereCenterZ;
    QMap<int, QStatistics<float>> sphereRadius;

    for (int i = 0; i < samples(); i++) {
        const Leap::Frame frame = controller.frame(i);

        Leap::HandList hands = frame.hands();
        for (int j = 0; j < hands.count(); j++) {
            Leap::Hand hand = hands[j];
            int id = hand.id();
            if (ids.contains(id)) {
                ids[id]++;
            } else {
                ids[id] = 1;
            }
            Leap::Vector palmPosition = hand.palmPosition();
            palmPositionX[id].append(palmPosition.x);
            palmPositionY[id].append(palmPosition.y);
            palmPositionZ[id].append(palmPosition.z);
            Leap::Vector palmVelocity = hand.palmVelocity();
            palmVelocityX[id].append(palmVelocity.x);
            palmVelocityY[id].append(palmVelocity.y);
            palmVelocityZ[id].append(palmVelocity.z);
            Leap::Vector palmNormal = hand.palmNormal();
            palmNormalX[id].append(palmNormal.x);
            palmNormalY[id].append(palmNormal.y);
            palmNormalZ[id].append(palmNormal.z);
            Leap::Vector direction = hand.direction();
            directionX[id].append(direction.x);
            directionY[id].append(direction.y);
            directionZ[id].append(direction.z);
            Leap::Vector sphereCenter = hand.sphereCenter();
            sphereCenterX[id].append(sphereCenter.x);
            sphereCenterY[id].append(sphereCenter.y);
            sphereCenterZ[id].append(sphereCenter.z);
            sphereRadius[id].append(hand.sphereRadius());
        }
    }

    beginUpdate();
    foreach (int id, ids.keys()) {
        if (ids.value(id) < samples() / 2) continue;
        QVariantList data;
        data.insert(Private::IdRole, id);
        data.insert(Private::PalmPositionRole, QVector3D(palmPositionX.value(id).average(), palmPositionY.value(id).average(), palmPositionZ.value(id).average()));
        data.insert(Private::PalmVelocityRole, QVector3D(palmVelocityX.value(id).average(), palmVelocityY.value(id).average(), palmVelocityZ.value(id).average()));
        data.insert(Private::PalmNormalRole, QVector3D(palmNormalX.value(id).average(), palmNormalY.value(id).average(), palmNormalZ.value(id).average()));
        data.insert(Private::DirectionRole, QVector3D(directionX.value(id).average(), directionY.value(id).average(), directionZ.value(id).average()));
        data.insert(Private::SphereCenterRole, QVector3D(sphereCenterX.value(id).average(), sphereCenterY.value(id).average(), sphereCenterZ.value(id).average()));
        data.insert(Private::SphereRadiusRole, sphereRadius.value(id).average());
        upsert(id, data);
    }
    endUpdate();
}

