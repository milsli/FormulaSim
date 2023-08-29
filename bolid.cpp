#include "bolid.h"

#include <iostream>
#include <random>


int Bolid::objectCounter = 0;
Bolid::Bolid(QObject *parent) : QObject(parent)
  , name_("F1")
  , start_(100)
  , speed_(100)
  , color_(QColor(0xff0000))
  , x_(0)
  , y_(45)
  , distance_(0)
  , currentLap_(1)
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
    }

    emit bolidDefinition(QVariant(bolidNumber_), QVariant(name_), QVariant(color_));
}

int Bolid::getCurrentLap() const
{
    return currentLap_;
}

void Bolid::setLaps(int laps)
{
    laps_ = laps;
}

void Bolid::run()
{
    // distance_ += 50 + rand() % start_;
    // ewentualnie
    distance_ += static_cast<int>(((((rand() % 8) + 2) + 100) * start_) / 100);

    timerId_ = startTimer(50);
}

void Bolid::fatum()
{
    int rnd = rand() % 10'000;

    if(rnd < 30)                        // crash
    {
        distance_ = -1;
    }
    else if(rnd > 500 && rnd < 1000)    // lost distance
    {
        distance_ -= rnd;
    }

}

void Bolid::timerEvent(QTimerEvent *event)
{
//    distance_ += 100 + rand() % speed_;
    // ewentualnie
    int rnd = rand();
    distance_ += static_cast<int>((((rnd % 20) + 100) * speed_) / 100);

    emit moveBolid(QVariant(bolidNumber_),QVariant(distance_));

    if(currentLap_ * 2000 <= distance_)
    {
        // fatum();
        if(distance_ != -1)
        {
            ++currentLap_;
            emit newLap(name_, currentLap_);
        }
        else if(distance_ == -1)
        {
            emit crash(name_);
            killTimer(timerId_);
        }
    }

    if(currentLap_ >= laps_)
        killTimer(timerId_);
}

void Bolid::onStartSignal()
{
    run();
}
