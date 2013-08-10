#include <QtQuick>

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    QQuickView view(QUrl("qrc:/leap.qml"));
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    QObject::connect(view.engine(), &QQmlEngine::quit, &view, &QQuickView::close);
    view.show();

    return app.exec();
}

