#include "supervisor.h"
#include <QFile>
#include "bolid.h"
#include "qqmllist.h"

#include <iostream>

Supervisor::Supervisor(QObject *parent)
    : QObject{parent}
    , configPath_("../config/config.cfg")
    , currentRaceLap_(1)
    , laps_(40)
{
}

Supervisor::~Supervisor()
{
}

void Supervisor::readConfig()
{
    QFile file(configPath_);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    while (!file.atEnd()) {
        QString line = file.readLine();

        processLine(line);
    }
}

QList<QVariant> Supervisor::rankingTable()
{
    return result_;
}

QQmlListProperty<CrashedCars> Supervisor::crashedCars()
{
    return {this, &crashed_};
}


void Supervisor::processLine(QString &line)
{
    if(line.trimmed()[0] == '#')
        return;

    QStringList configList = line.split(" ");

    if(line.contains("Bolid"))
    {
        Bolid *bolid = new Bolid(this);

        connect(bolid, &Bolid::bolidDefinition, this, &Supervisor::bolidDefinition);
        connect(this, &Supervisor::startSignal, bolid, &Bolid::onStartSignal);
        connect(bolid, &Bolid::moveBolid, this, &Supervisor::onMoveBolid);
        connect(bolid, &Bolid::newLap, this, &Supervisor::onNewLap);
        connect(bolid, &Bolid::crash, this, &Supervisor::onCrash);

        bolid->configure(configList);
        bolids_.push_back(bolid);
    }
    else if(line.contains("Race"))
    {
        setRaceConfig(configList);
    }
}

void Supervisor::setRaceConfig(const QStringList conf)
{
    for(int i = 0; i < conf.size(); ++i)
    {
        if(conf[i].contains("-round")) {
            laps_ = conf[i + 1].toInt();
        }
    }

    for(Bolid *b : bolids_)
    {
        b->setLaps(laps_);
    }

    emit newLap(currentRaceLap_, laps_);
}

void Supervisor::onNewLap(QString name, int currentLap)
{
    auto it = std::find_if_not(bolids_.begin(), bolids_.end(), [=](Bolid *bolid){return bolid->getCurrentLap() > currentRaceLap_;});
    if(it != bolids_.end())
        return;

    ++currentRaceLap_;

    std::sort(bolids_.begin(), bolids_.end(), [=](Bolid *b1, Bolid *b2){return b1->getDistance() > b2->getDistance();});

    for(Bolid *bolid : bolids_)
        result_.append(bolid->getName() + "  >  " + QString::number(bolid->getDistance()));

    emit newLap(currentRaceLap_, laps_);
    emit showRanking(); // wysyła result_

    if(currentRaceLap_ > laps_)
    {
        for(Bolid *bolid : bolids_)
        {
            bolid->killBolidTimer();
        }
    }

    result_.clear();
}

void Supervisor::onMoveBolid(QVariant bolidNumber, QVariant distance)
{
    int maxDistance = LAP_DISTANCE * laps_;
    int xDistance = static_cast<int>(1920 * distance.toInt() / ( 1.3 * maxDistance ));

    emit moveBolid(bolidNumber, xDistance);
}

void Supervisor::onCrash(QString name, QVariant bolidNumber)
{
    auto it = std::find_if(bolids_.begin(), bolids_.end(), [=](Bolid *bolid){return bolid->getName() == name;});

    if(it == bolids_.end())
            return;

    int number = (*it)->getBolidNumber();
    crashedBolids_.push_back(*it);
    bolids_.erase(it);

    crashed_.append(new CrashedCars(name, number));

    emit crashedCarsSignal(bolidNumber);
    //    emit crashedCarsSignalBis();
}

void Supervisor::onNewRace()
{
    currentRaceLap_ = 1;
    result_.clear();
    crashed_.clear();

    for(auto it = crashedBolids_.begin(); it != crashedBolids_.end(); ++it)
    {
        bolids_.push_back(*it);
    }

    for(Bolid *bolid : bolids_)
    {
        bolid->resetDistance();
    }

    crashedBolids_.clear();
    emit crashedCarsSignal(-1);
}

//QQmlListProperty<CrashedCars> Supervisor::crashedCarsBis()
//{
//    CrashedCars *c1 = new CrashedCars();
//    c1->name_ = "fir";
//    c1->number_ = 6;
//    crashedCarsBB.append(c1);


//    CrashedCars *c2 = new CrashedCars();
//    c2->name_ = "second";
//    c2->number_ = 67;
//    crashedCarsBB.append(c2);

//    return {this, &crashedCarsBB};
//}
