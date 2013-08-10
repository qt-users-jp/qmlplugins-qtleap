#include "qabstractleaplistenermodel.h"

class QAbstractLeapListenerModel::Private
{
public:
    Private();
    int samples;
};

QAbstractLeapListenerModel::Private::Private()
    : samples(3)
{
}

QAbstractLeapListenerModel::QAbstractLeapListenerModel(QObject *parent)
    : QAbstractLeapModel(parent)
    , d(new Private())
{
}

void QAbstractLeapListenerModel::timeout(const Leap::Controller &controller)
{
    onFrame(controller);
}

int QAbstractLeapListenerModel::samples() const
{
    return d->samples;
}

void QAbstractLeapListenerModel::setSamples(int samples)
{
    if (samples < 1) {
        qWarning() << tr("Samples must be greater than 0");
        return;
    }
    if (d->samples == samples) return;
    d->samples = samples;
    emit samplesChanged(samples);
}
