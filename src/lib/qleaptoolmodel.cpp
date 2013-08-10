#include "qleaptoolmodel.h"

QLeapToolModel::QLeapToolModel(QObject *parent)
    : QAbstractLeapPointableModel(parent)
{
}

static Leap::ToolList pointables(const Leap::Frame &frame) {
    return frame.tools();
}

void QLeapToolModel::onFrame(const Leap::Controller &controller)
{
    onFrame2<Leap::ToolList>(controller, &pointables);
}
