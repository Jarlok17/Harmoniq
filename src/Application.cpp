#include <qsgrendererinterface.h>

#include "../configs/precompiled.hpp"
#include "layer/LayerManager.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setOrganizationName("Harmoniq");
    app.setApplicationName("harmoniq");
    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;
    engine.addImportPath("qrc:/Styles");
    // registre type layer
    qmlRegisterType<harmoniq::layer::Layer>("Harmoniq_backend", 1, 0, "Layer");

    // root context
    harmoniq::layer::LayerManager layerManager;
    engine.rootContext()->setContextProperty("layerManager", &layerManager);

    const QUrl url(QStringLiteral("qrc:/Qml/main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
