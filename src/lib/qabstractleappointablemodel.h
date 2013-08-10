#ifndef QABSTRACTLEAPPOINTABLEMODEL_H
#define QABSTRACTLEAPPOINTABLEMODEL_H

#include "qabstractleaplistenermodel.h"
#include "qstatistics_p.h"

class QTLEAP_EXPORT QAbstractLeapPointableModel : public QAbstractLeapListenerModel
{
    Q_OBJECT
    Q_ENUMS(Zone)
public:
    enum Zone {
        ZoneNone,
        ZoneHovering,
        ZoneTouching
    };

    explicit QAbstractLeapPointableModel(QObject *parent = 0);

    virtual QHash<int, QByteArray> roleNames() const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    enum Role {
        IdRole = 0
        , TipPositionRole
        , TipVelocityRole
        , DirectionRole
        , WidthRole
        , LengthRole
        , TouchZoneRole
        , TouchDistanceRole
        , HandIdRole
    };
    template <class T>
    void onFrame2(const Leap::Controller &controller, T (*func)(const Leap::Frame &frame)) {
        QMap<int, int> ids;
        QMap<int, QStatistics<float>> tipPositionX;
        QMap<int, QStatistics<float>> tipPositionY;
        QMap<int, QStatistics<float>> tipPositionZ;
        QMap<int, QStatistics<float>> tipVelocityX;
        QMap<int, QStatistics<float>> tipVelocityY;
        QMap<int, QStatistics<float>> tipVelocityZ;
        QMap<int, QStatistics<float>> directionX;
        QMap<int, QStatistics<float>> directionY;
        QMap<int, QStatistics<float>> directionZ;
        QMap<int, QStatistics<float>> width;
        QMap<int, QStatistics<float>> length;
        QMap<int, int> touchZone;
        QMap<int, QStatistics<float>> touchDistance;
        QMap<int, int> handId;

        for (int i = 0; i < samples(); i++) {
            Leap::Frame frame = controller.frame(i);

            T pointables = func(frame);
            for (int j = 0; j < pointables.count(); j++) {
                Leap::Pointable pointable = pointables[j];
                int id = pointable.id();
                if (ids.contains(id)) {
                    ids[id]++;
                } else {
                    ids.insert(id, 1);
                    handId.insert(id, pointable.hand().id());
                    touchZone.insert(id, pointable.touchZone());
                }
                Leap::Vector tipPosition = pointable.tipPosition();
                tipPositionX[id].append(tipPosition.x);
                tipPositionY[id].append(tipPosition.y);
                tipPositionZ[id].append(tipPosition.z);
                Leap::Vector tipVelocity = pointable.tipVelocity();
                tipVelocityX[id].append(tipVelocity.x);
                tipVelocityY[id].append(tipVelocity.y);
                tipVelocityZ[id].append(tipVelocity.z);
                Leap::Vector direction = pointable.direction();
                directionX[id].append(direction.x);
                directionY[id].append(direction.y);
                directionZ[id].append(direction.z);
                width[id].append(pointable.width());
                length[id].append(pointable.length());
                touchDistance[id].append(pointable.touchDistance());
            }
        }

        beginUpdate();
        foreach (int id, ids.keys()) {
            if (ids.value(id) < samples() / 2) continue;
            QVariantList data;
            data.insert(IdRole, id);
            data.insert(TipPositionRole, QVector3D(tipPositionX.value(id).average(), tipPositionY.value(id).average(), tipPositionZ.value(id).average()));
            data.insert(TipVelocityRole, QVector3D(tipVelocityX.value(id).average(), tipVelocityY.value(id).average(), tipVelocityZ.value(id).average()));
            data.insert(DirectionRole, QVector3D(directionX.value(id).average(), directionY.value(id).average(), directionZ.value(id).average()));
            data.insert(WidthRole, width.value(id).average());
            data.insert(LengthRole, length.value(id).average());
            data.insert(TouchZoneRole, (Zone)touchZone.value(id));
            data.insert(TouchDistanceRole, touchDistance.value(id).average());
            data.insert(HandIdRole, handId.value(id));
            upsert(id, data);
        }
        endUpdate();
    }
private:
    class Private;
    Private *d;
};

#endif // QABSTRACTLEAPPOINTABLEMODEL_H
