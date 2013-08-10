#ifndef QLEAPTOOLMODEL_H
#define QLEAPTOOLMODEL_H

#include "qabstractleappointablemodel.h"

class QTLEAP_EXPORT QLeapToolModel : public QAbstractLeapPointableModel
{
    Q_OBJECT
public:
    explicit QLeapToolModel(QObject *parent = 0);
    
protected:
    virtual void onFrame(const Leap::Controller &controller);
};

#endif // QLEAPTOOLMODEL_H
