#ifndef QABSTRACTLEAPLISTENERMODEL_H
#define QABSTRACTLEAPLISTENERMODEL_H

#include "qabstractleapmodel.h"

class QTLEAP_EXPORT QAbstractLeapListenerModel : public QAbstractLeapModel, protected Leap::Listener
{
    Q_OBJECT
    Q_PROPERTY(int samples READ samples WRITE setSamples NOTIFY samplesChanged)
public:
    explicit QAbstractLeapListenerModel(QObject *parent = 0);

    int samples() const;

public slots:
    void setSamples(int samples);

signals:
    void samplesChanged(int samples);

protected:
    virtual void timeout(const Leap::Controller &controller);

private:
    class Private;
    Private *d;
};

#endif // QABSTRACTLEAPLISTENERMODEL_H
