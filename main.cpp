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
    qmlRegisterAnonymousType<CrashedCars>("uri",6); //(<CrashedCars>(/*"uri", 0, 1, "CrashedCars"*/);

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
    QObject::connect(supervisor, SIGNAL(newRace(QVariant,QVariant)), engine.rootObjects().at(0), SLOT(onNewRace(QVariant,QVariant)));
    QObject::connect(supervisor, SIGNAL(newLap(QVariant)), engine.rootObjects().at(0), SLOT(newLap(QVariant)));
    QObject::connect(supervisor, SIGNAL(moveBolid(QVariant,QVariant)), engine.rootObjects().at(0), SLOT(moveBolid(QVariant,QVariant)));
    QObject::connect(supervisor, SIGNAL(crashedCarsSignal(QVariant)), engine.rootObjects().at(0), SLOT(onCrashedCar(QVariant)));

    QObject::connect(engine.rootObjects().at(0), SIGNAL(startSignal()), supervisor, SIGNAL(startSignal()));
    QObject::connect(engine.rootObjects().at(0), SIGNAL(newRace()), supervisor, SLOT(onNewRace()));

    supervisor->readConfig();


    return app.exec();
}
