#include "qleapfingermodel.h"

QLeapFingerModel::QLeapFingerModel(QObject *parent)
    : QAbstractLeapPointableModel(parent)
{
}

static Leap::FingerList pointables(const Leap::Frame &frame) {
    return frame.fingers();
}

void QLeapFingerModel::onFrame(const Leap::Controller &controller)
{
    onFrame2<Leap::FingerList>(controller, &pointables);
}


