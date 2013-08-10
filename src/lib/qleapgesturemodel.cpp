#include "qleapgesturemodel.h"

#include "qstatistics_p.h"

class QLeapGestureModel::Private
{
public:
    enum Role {
        IdRole = 0
        , TypeRole
        , StateRole
        , DurationRole
        , DurationSecondsRole
        // For Swipe
        , StartPositionRole
        , PositionRole
        , DirectionRole
        , SpeedRole
        // For Circle
        , CenterRole
        , NormalRole
        , ProgressRole
        , RadiusRole
    };

    Private(QLeapGestureModel *parent);

private:
    QLeapGestureModel *q;

public:
    GestureTypes gestureTypes;
};

QLeapGestureModel::Private::Private(QLeapGestureModel *parent)
    : q(parent)
    , gestureTypes(0)
{
    q->connect(q, &QLeapGestureModel::gestureTypesChanged, [this](QLeapGestureModel::GestureTypes gestureTypes) {
        Leap::Controller &controller = q->controller();
        controller.enableGesture(Leap::Gesture::TYPE_SWIPE, gestureTypes & SwipeGesture);
        controller.enableGesture(Leap::Gesture::TYPE_CIRCLE, gestureTypes & CircleGesture);
        controller.enableGesture(Leap::Gesture::TYPE_SCREEN_TAP, gestureTypes & ScreenTapGesture);
        controller.enableGesture(Leap::Gesture::TYPE_KEY_TAP, gestureTypes & KeyTapGesture);
    });
}

QLeapGestureModel::QLeapGestureModel(QObject *parent)
    : QAbstractLeapListenerModel(parent)
    , d(new Private(this))
{
}

QHash<int, QByteArray> QLeapGestureModel::roleNames() const
{
    static QHash<int, QByteArray> ret;
    if (ret.isEmpty()) {
        ret.insert(Qt::UserRole + Private::IdRole, "id");
        ret.insert(Qt::UserRole + Private::TypeRole, "type");
        ret.insert(Qt::UserRole + Private::StateRole, "state");
        ret.insert(Qt::UserRole + Private::DurationRole, "duration");
        ret.insert(Qt::UserRole + Private::DurationSecondsRole, "duratoinSeconds");
        ret.insert(Qt::UserRole + Private::StartPositionRole, "startPosition");
        ret.insert(Qt::UserRole + Private::PositionRole, "position");
        ret.insert(Qt::UserRole + Private::DirectionRole, "direction");
        ret.insert(Qt::UserRole + Private::SpeedRole, "speed");
        ret.insert(Qt::UserRole + Private::CenterRole, "center");
        ret.insert(Qt::UserRole + Private::NormalRole, "normal");
        ret.insert(Qt::UserRole + Private::ProgressRole, "progress");
        ret.insert(Qt::UserRole + Private::RadiusRole, "radius");
    }
    return ret;
}

QVariant QLeapGestureModel::data(const QModelIndex &index, int role) const
{
    QVariant ret;
    switch (role) {
    case Qt::DisplayRole:
        ret = QStringLiteral("Gesture {id:%1}").arg(QAbstractLeapListenerModel::data(index, Qt::UserRole + Private::IdRole).toInt());
        break;
    default:
        ret = QAbstractLeapListenerModel::data(index, role);
//        qDebug() << Q_FUNC_INFO << __LINE__ << role - Qt::UserRole << ret;
        break;
    }

    return ret;
}

void QLeapGestureModel::onFrame(const Leap::Controller &controller)
{
    QMap<int, int> ids;
    QMap<int, GestureType> type;
    QMap<int, GestureState> state;
    QMap<int, QStatistics<qint64>> duration;
    QMap<int, QStatistics<float>> durationSeconds;
    QMap<int, QStatistics<float>> startPositionX;
    QMap<int, QStatistics<float>> startPositionY;
    QMap<int, QStatistics<float>> startPositionZ;
    QMap<int, QStatistics<float>> positionX;
    QMap<int, QStatistics<float>> positionY;
    QMap<int, QStatistics<float>> positionZ;
    QMap<int, QStatistics<float>> directionX;
    QMap<int, QStatistics<float>> directionY;
    QMap<int, QStatistics<float>> directionZ;
    QMap<int, QStatistics<float>> speed;
    QMap<int, QStatistics<float>> centerX;
    QMap<int, QStatistics<float>> centerY;
    QMap<int, QStatistics<float>> centerZ;
    QMap<int, QStatistics<float>> normalX;
    QMap<int, QStatistics<float>> normalY;
    QMap<int, QStatistics<float>> normalZ;
    QMap<int, QStatistics<float>> radius;
    QMap<int, QStatistics<float>> progress;

    for (int i = 0; i < samples(); i++) {
        const Leap::Frame frame = controller.frame(i);

        Leap::GestureList gestures = frame.gestures();
        for (int j = 0; j < gestures.count(); j++) {
            Leap::Gesture gesture = gestures[j];
            int id = gesture.id();
            if (ids.contains(id)) {
                ids[id]++;
            } else {
                ids.insert(id, 1);
                switch (gesture.type()) {
                case Leap::Gesture::TYPE_SWIPE:
                    type.insert(id, SwipeGesture);
                    break;
                case Leap::Gesture::TYPE_CIRCLE:
                    type.insert(id, CircleGesture);
                    break;
                case Leap::Gesture::TYPE_SCREEN_TAP:
                    type.insert(id, ScreenTapGesture);
                    break;
                case Leap::Gesture::TYPE_KEY_TAP:
                    type.insert(id, KeyTapGesture);
                    break;
                default:
                    type.insert(id, InvalidGesture);
                    break;
                }
                switch (gesture.state()) {
                case Leap::Gesture::STATE_START:
                    state.insert(id, StartState);
                    break;
                case Leap::Gesture::STATE_UPDATE:
                    state.insert(id, UpdateState);
                    break;
                case Leap::Gesture::STATE_STOP:
                    state.insert(id, StopState);
                    break;
                default:
                    state.insert(id, InvalidState);
                    break;
                }

            }
            duration[id].append(gesture.duration());
            durationSeconds[id].append(gesture.durationSeconds());

            switch (gesture.type()) {
            case Leap::Gesture::TYPE_SWIPE: {
                Leap::SwipeGesture swipe = gesture;
                Leap::Vector startPosition = swipe.startPosition();
                startPositionX[id].append(startPosition.x);
                startPositionY[id].append(startPosition.y);
                startPositionZ[id].append(startPosition.z);
                Leap::Vector position = swipe.position();
                positionX[id].append(position.x);
                positionY[id].append(position.y);
                positionZ[id].append(position.z);
                Leap::Vector direction = swipe.direction();
                directionX[id].append(direction.x);
                directionY[id].append(direction.y);
                directionZ[id].append(direction.z);
                speed[id].append(swipe.speed());
                break; }
            case Leap::Gesture::TYPE_CIRCLE: {
                Leap::CircleGesture circle = gesture;
                Leap::Vector center = circle.center();
                centerX[id].append(center.x);
                centerY[id].append(center.y);
                centerZ[id].append(center.z);
                Leap::Vector normal = circle.normal();
                normalX[id].append(normal.x);
                normalY[id].append(normal.y);
                normalZ[id].append(normal.z);
                progress[id].append(circle.progress());
                radius[id].append(circle.radius());
                break; }
            case Leap::Gesture::TYPE_SCREEN_TAP: {
                Leap::ScreenTapGesture screenTap = gesture;
                Leap::Vector position = screenTap.position();
                positionX[id].append(position.x);
                positionY[id].append(position.y);
                positionZ[id].append(position.z);
                Leap::Vector direction = screenTap.direction();
                directionX[id].append(direction.x);
                directionY[id].append(direction.y);
                directionZ[id].append(direction.z);
                progress[id].append(screenTap.progress());
                break; }
            case Leap::Gesture::TYPE_KEY_TAP: {
                Leap::KeyTapGesture keyTap = gesture;
                Leap::Vector position = keyTap.position();
                positionX[id].append(position.x);
                positionY[id].append(position.y);
                positionZ[id].append(position.z);
                Leap::Vector direction = keyTap.direction();
                directionX[id].append(direction.x);
                directionY[id].append(direction.y);
                directionZ[id].append(direction.z);
                progress[id].append(keyTap.progress());
                break; }
            default:
                break;
            }
        }
    }

    beginUpdate();
    foreach (int id, ids.keys()) {
        if (ids.value(id) < samples() / 2) continue;
        QVariantList data;
        data.insert(Private::IdRole, id);
        data.insert(Private::TypeRole, type.value(id));
        data.insert(Private::StateRole, state.value(id));
        data.insert(Private::DurationRole, duration.value(id).average());
        data.insert(Private::DurationSecondsRole, durationSeconds.value(id).average());
        data.insert(Private::StartPositionRole, QVector3D(startPositionX.value(id).average(), startPositionY.value(id).average(), startPositionZ.value(id).average()));
        data.insert(Private::PositionRole, QVector3D(positionX.value(id).average(), positionY.value(id).average(), positionZ.value(id).average()));
        data.insert(Private::DirectionRole, QVector3D(directionX.value(id).average(), directionY.value(id).average(), directionZ.value(id).average()));
        data.insert(Private::SpeedRole, speed.value(id).average());
        data.insert(Private::CenterRole, QVector3D(centerX.value(id).average(), centerY.value(id).average(), centerZ.value(id).average()));
        data.insert(Private::NormalRole, QVector3D(normalX.value(id).average(), normalY.value(id).average(), normalZ.value(id).average()));
        data.insert(Private::ProgressRole, progress.value(id).average());
        data.insert(Private::RadiusRole, radius.value(id).average());
        upsert(id, data);
    }
    endUpdate();
}

QLeapGestureModel::GestureTypes QLeapGestureModel::gestureTypes() const
{
    return d->gestureTypes;
}

void QLeapGestureModel::setGestureTypes(QLeapGestureModel::GestureTypes gestureTypes)
{
    if (d->gestureTypes == gestureTypes) return;

    d->gestureTypes = gestureTypes;
    emit gestureTypesChanged(gestureTypes);
}
