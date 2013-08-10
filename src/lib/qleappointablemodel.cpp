#include "qleappointablemodel.h"

QLeapPointableModel::QLeapPointableModel(QObject *parent)
    : QAbstractLeapPointableModel(parent)
{
}

static Leap::PointableList pointables(const Leap::Frame &frame) {
    return frame.pointables();
}

void QLeapPointableModel::onFrame(const Leap::Controller &controller)
{
    onFrame2<Leap::PointableList>(controller, &pointables);
}
