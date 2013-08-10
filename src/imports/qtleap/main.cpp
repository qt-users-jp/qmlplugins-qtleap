#include <QtQml/QQmlExtensionPlugin>
#include <QtQml/qqml.h>
#include <QtQml/private/qqmlchangeset_p.h>

#include <QtLeap/QLeapHandModel>
#include <QtLeap/QLeapPointableModel>
#include <QtLeap/QLeapFingerModel>
#include <QtLeap/QLeapToolModel>
#include <QtLeap/QLeapGestureModel>

class LeapGestureModel : public QLeapGestureModel
{
    Q_OBJECT
    Q_PROPERTY(QVariantList gestures READ gestures WRITE setGestures NOTIFY gesturesChanged)
public:
    LeapGestureModel(QObject *parent = 0) : QLeapGestureModel(parent) {
        connect(this, &QLeapGestureModel::gestureTypesChanged, this, &LeapGestureModel::gesturesChanged);
    }

    QVariantList gestures() const {
        QVariantList ret;
        for (int i = SwipeGesture; i <= KeyTapGesture; i *= 2) {
            if (i & gestureTypes()) ret.append(static_cast<GestureType>(i));
        }
        return ret;
    }

public slots:
    void setGestures(const QVariantList &gestures) {
        if (this->gestures() == gestures) return;
        GestureTypes gestureTypes = 0;
        foreach (const QVariant &gestureType, gestures) {
            gestureTypes |= static_cast<GestureType>(gestureType.toInt());
        }
        setGestureTypes(gestureTypes);
    }

signals:
    void gesturesChanged();
};

class QmlLeapPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    virtual void registerTypes(const char *uri)
    {
        qRegisterMetaType<QQmlChangeSet>("QQmlChangeSet");

        Q_ASSERT(QLatin1String(uri) == QLatin1String("QtLeap"));
        // @uri QtLeap
        qmlRegisterType<QLeapHandModel>(uri, 0, 1, "HandModel");
        qmlRegisterType<QLeapFingerModel>(uri, 0, 1, "PointableModel");
        qmlRegisterType<QLeapFingerModel>(uri, 0, 1, "FingerModel");
        qmlRegisterType<QLeapToolModel>(uri, 0, 1, "ToolModel");
        qmlRegisterType<LeapGestureModel>(uri, 0, 1, "GestureModel");
    }
};

#include "main.moc"
