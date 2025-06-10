#include "../tools/ToolSettings.hpp"
#include "document/DocumentManager.hpp"
#include "layer/Layer.hpp"
#include "tools/ToolHandler.hpp"

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
    qmlRegisterSingletonType<harmoniq::ToolSettings>(
        "Harmoniq_backend", 1, 0, "ToolSettings",
        [](QQmlEngine *, QJSEngine *) -> QObject * { return harmoniq::ToolSettings::instance(); });

    qmlRegisterType<harmoniq::layer::Layer>("Harmoniq_backend", 1, 0, "Layer");

    harmoniq::document::DocumentManager *documentManager = new harmoniq::document::DocumentManager();
    engine.rootContext()->setContextProperty("documentManager", documentManager);

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
