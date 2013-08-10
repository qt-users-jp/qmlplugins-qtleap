#ifndef QLEAPHANDMODEL_H
#define QLEAPHANDMODEL_H

#include "qabstractleaplistenermodel.h"

class QTLEAP_EXPORT QLeapHandModel : public QAbstractLeapListenerModel
{
    Q_OBJECT
public:
    explicit QLeapHandModel(QObject *parent = 0);
    
    virtual QHash<int, QByteArray> roleNames() const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    void onFrame(const Leap::Controller &controller);

private:
    class Private;
    Private *d;
};

#endif // QLEAPHANDMODEL_H
