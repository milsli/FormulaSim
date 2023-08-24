#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "supervisor.h"
#include <QQmlContext>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    QObject obj;
    Supervisor *supervisor = new Supervisor(&obj);
    QQmlApplicationEngine engine;

    QQmlContext *context = engine.rootContext();
    context->setContextProperty("appCore", supervisor);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);


    //QObject::connect(supervisor, &Supervisor::bolidDefinition, &engine, &QQmlApplicationEngine::onSendToQml);

    engine.load(url);


    return app.exec();
}
