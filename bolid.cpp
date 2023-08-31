#include "bolid.h"

#include <iostream>
#include <random>


int Bolid::objectCounter = 0;
Bolid::Bolid(QObject *parent) : QObject(parent)
  , name_("F1")
  , start_(100)
  , speed_(100)
  , color_(QColor(0xff0000))
  , distance_(0)
  , currentLap_(1)
  , delayCounter_(0)
  , suspended_(0)
  , failure_(0)
  , allowMove_(true)
{
    ++objectCounter;
    bolidNumber_ = objectCounter;
    srand(time(nullptr));
}

void Bolid::configure(const QStringList argv)
{
    for(int i = 0; i < argv.size(); ++i)
    {
        if(argv[i].contains("-driver")) {
            name_ = argv[i + 1];
        }
        if(argv[i].contains("-start")) {
            start_ = argv[i + 1].toInt();
        }
        if(argv[i].contains("-speed")) {
            speed_ = argv[i + 1].toInt();
        }
        if(argv[i].contains("-color")) {
            bool b;
            color_ = argv[i + 1].toInt(&b,16);
        }
        if(argv[i].contains("-failure")) {
            failure_ = argv[i + 1].toInt();
        }
    }

    emit bolidDefinition(QVariant(bolidNumber_), QVariant(name_), QVariant(color_));
}

int Bolid::getCurrentLap() const
{
    if(!allowMove_)
        return currentLap_ + 1;

    return currentLap_;
}

void Bolid::setLaps(int laps)
{
    laps_ = laps;

    lapsBorders_.resize(laps_);
    for(int i = 0; i < laps_; ++i)
    {
        lapsBorders_[i] = (i + 1) * LAP_DISTANCE;
    }
}

void Bolid::killBolidTimer()
{
    killTimer(timerId_);
}

void Bolid::run()
{
    // distance_ += 50 + rand() % start_;
    // ewentualnie
    distance_ += static_cast<int>(((((rand() % 10) + 2) + 100) * start_) / 100);

    timerId_ = startTimer(100);
}

void Bolid::fatum()
{
    if(!allowMove_)
        return;

    int rnd = rand() % 10'000;

    if(rnd < 90)                        // crash
    {
        rnd = rand() % 100;
        if(rnd < failure_)
            distance_ = -1;
    }
    else if(rnd > 550 && rnd < 1000)    // lost distance
    {
        rnd = 5 + rand() % 12;
        delayCounter_ = rnd;
    }
}

void Bolid::timerEvent(QTimerEvent *event)
{
//    distance_ += 100 + rand() % speed_;
    // ewentualnie

    if(!(suspended_ % 2))
        return;

    if(delayCounter_ == 0 && allowMove_)
    {
        int rnd = rand();
        distance_ += static_cast<int>((((rnd % 20) + 100) * speed_) / 100);
        emit moveBolid(QVariant(bolidNumber_),QVariant(distance_));
    }
    else
        --delayCounter_;


    if(lapsBorders_[currentLap_ - 1] < distance_)
    {
        fatum();
        if(distance_ != -1)
        {
            ++currentLap_;
            if(currentLap_ > laps_)
            {
                distance_ += 120;
                --currentLap_;
                allowMove_ = false;
            }
            emit newLap(name_, currentLap_ + (allowMove_ ? 0 : 1));
        }
        else if(distance_ == -1)
        {
            emit crash(name_);
            killTimer(timerId_);
        }
    }
}

void Bolid::onStartSignal()
{
    ++suspended_;
    if(suspended_ == 1)
        run();
}
