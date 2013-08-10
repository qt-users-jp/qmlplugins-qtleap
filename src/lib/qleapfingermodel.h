#ifndef QLEAPFINGERMODEL_H
#define QLEAPFINGERMODEL_H

#include "qabstractleappointablemodel.h"

class QTLEAP_EXPORT QLeapFingerModel : public QAbstractLeapPointableModel
{
    Q_OBJECT
public:
    explicit QLeapFingerModel(QObject *parent = 0);

protected:
    virtual void onFrame(const Leap::Controller &controller);
};

#endif // QLEAPFINGERMODEL_H
