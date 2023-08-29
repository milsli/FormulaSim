#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "supervisor.h"

// using namespace Race;


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    qRegisterMetaType<QList<QString>>();

    Supervisor *supervisor = new Supervisor;

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.rootContext()->setContextProperty("supervisor", supervisor);
    engine.load(url);


    QObject::connect(supervisor, SIGNAL(bolidDefinition(QVariant,QVariant,QVariant)), engine.rootObjects().at(0), SLOT(createBolid(QVariant,QVariant,QVariant)));
    QObject::connect(supervisor, SIGNAL(moveBolid(QVariant,QVariant)), engine.rootObjects().at(0), SLOT(moveBolid(QVariant,QVariant)));
    QObject::connect(supervisor, SIGNAL(newLap(QVariant,QVariant)), engine.rootObjects().at(0), SLOT(newLap(QVariant,QVariant)));

    // QObject::connect(supervisor, SIGNAL(showRanking(QVector<QString>)), engine.rootObjects().at(0), SLOT(rr(QVariantList)));

   // QObject::connect(engine.rootObjects().at(0), SIGNAL(startSignal()), supervisor, SLOT(onStartSignal()));
    QObject::connect(engine.rootObjects().at(0), SIGNAL(startSignal()), supervisor, SIGNAL(startSignal()));

    supervisor->readConfig();


    return app.exec();
}
