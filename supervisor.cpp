#include "supervisor.h"
#include <QFile>
#include "bolid.h"
#include "qqmllist.h"

#include <iostream>

Supervisor::Supervisor(QObject *parent)
    : QObject{parent}
    , configPath_("../config/config.cfg")
    , currentRaceLap_(1)
    , raceNumber_(1)
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

    while (!file.atEnd())
    {
        QString line = file.readLine();
        processLine(line);
    }
}

QList<QVariant> Supervisor::rankingTable()
{
    return result_;
}

QList<QVariant> Supervisor::classificationTable()
{

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
        if(conf[i].contains("-name")) {
            raceName_.append(conf[i + 1]);
        }
        else if(conf[i].contains("-lenght")) {
            lapDistance_.append(conf[i + 1].toInt());
        }
        else if(conf[i].contains("-laps")) {
            laps_.append(conf[i + 1].toInt());
        }
    }

    setNewRaceForBolids(laps_[0], lapDistance_[0]);

    emit newRace(raceName_[0], laps_[0]);
    emit newLap(currentRaceLap_);
}

void Supervisor::setNewRaceForBolids(int laps, int lapDistance)
{
    for(Bolid *b : bolids_)
    {
        b->setRaceData(laps, lapDistance);
    }
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

    emit newLap(currentRaceLap_);
    emit showRanking(); // wysyła result_

    if(currentRaceLap_ > laps_[raceNumber_ - 1])
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
    int maxDistance = lapDistance_[raceNumber_ - 1] * laps_[raceNumber_ - 1];
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

    setNewRaceForBolids(laps_[raceNumber_], lapDistance_[raceNumber_]);

    crashedBolids_.clear();
    emit crashedCarsSignal(-1);

    emit newRace(raceName_[raceNumber_], laps_[raceNumber_]);
    ++raceNumber_;
}
