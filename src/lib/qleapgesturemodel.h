#ifndef QLEAPGESTUREMODEL_H
#define QLEAPGESTUREMODEL_H

#include "qabstractleaplistenermodel.h"

class QTLEAP_EXPORT QLeapGestureModel : public QAbstractLeapListenerModel
{
    Q_OBJECT
    Q_ENUMS(GestureType)
    Q_PROPERTY(GestureTypes gestureTypes READ gestureTypes WRITE setGestureTypes NOTIFY gestureTypesChanged)
public:
    enum GestureType {
        InvalidGesture = 0x00
        , SwipeGesture = 0x01
        , CircleGesture = 0x02
        , ScreenTapGesture = 0x04
        , KeyTapGesture = 0x08
        , AllGestures = SwipeGesture | CircleGesture | ScreenTapGesture | KeyTapGesture
    };
    Q_DECLARE_FLAGS(GestureTypes, GestureType)

    enum GestureState {
        InvalidState
        , StartState
        , UpdateState
        , StopState
    };
    explicit QLeapGestureModel(QObject *parent = 0);

    virtual QHash<int, QByteArray> roleNames() const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    GestureTypes gestureTypes() const;

public slots:
    void setGestureTypes(GestureTypes gestureTypes);

signals:
    void gestureTypesChanged(GestureTypes gestureTypes);

protected:
    void onFrame(const Leap::Controller &controller);

private:
    class Private;
    Private *d;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QLeapGestureModel::GestureTypes)

#endif // QLEAPGESTUREMODEL_H
