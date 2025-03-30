#include <qsgrendererinterface.h>

// #include "brushes/BrushManager.hpp"
#include "configs/precompiled.hpp"
#include "layer/Layer.hpp"
#include "layer/LayerManager.hpp"
#include "layer/Tools.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setOrganizationName("Harmoniq");
    app.setApplicationName("harmoniq");
    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;
    engine.addImportPath("qrc:/Styles");

    qmlRegisterUncreatableMetaObject(harmoniq::ToolWrapper::staticMetaObject, "harmoniq", 1, 0, "ToolType",
                                     "Enum ToolType не може бути створений в QML");
    qmlRegisterType<harmoniq::layer::Layer>("Harmoniq_backend", 1, 0, "Layer");

    harmoniq::layer::LayerManager *layerManager = new harmoniq::layer::LayerManager();
    // harmoniq::brushes::BrushManager *brushManager = new harmoniq::brushes::BrushManager();
    engine.rootContext()->setContextProperty("layerManager", layerManager);
    // engine.rootContext()->setContextProperty("brushManager", brushManager);

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
