#ifndef QLEAPPOINTABLEMODEL_H
#define QLEAPPOINTABLEMODEL_H

#include "qabstractleappointablemodel.h"

class QTLEAP_EXPORT QLeapPointableModel : public QAbstractLeapPointableModel
{
    Q_OBJECT
public:
    explicit QLeapPointableModel(QObject *parent = 0);

protected:
    virtual void onFrame(const Leap::Controller &controller);
};

#endif // QLEAPPOINTABLEMODEL_H
