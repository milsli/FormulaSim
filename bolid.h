#ifndef BOLID_H
#define BOLID_H

#include "qobject.h"
#include <QStringList>
#include <QColor>
#include <QVariant>

class Bolid : public QObject
{
    Q_OBJECT
public:
    static int objectCounter;

    Bolid(QObject *parent = nullptr);

    void configure(const QStringList argv);
    int getCurrentLap() const;
    void setRaceData(int laps, int lapDistance);
    int getDistance() const {return distance_;}
    QString getName() {return name_;}
    void killBolidTimer();
    int getBolidNumber() const {return bolidNumber_;}
    void resetDistance();
    void addClassificationPoints(const int points);
    int getClassificationPoints() {return classificationPoints_;};

//{distance_ = 0; suspended_ = 0; afterNew = true; delayCounter_=0;allowMove_=true;}

private:
    void run();
    void fatum();

protected:
    void timerEvent(QTimerEvent *event) override;

private:
    QString name_ = "F1";
    int start_ = 100;
    int speed_ = 100;
    QColor color_;
    int distance_;
    int laps_;
    int currentLap_;
    int delayCounter_;
    int suspended_;
    int failure_;
    int driverAbility_;
    bool allowMove_;
    int lapDistance_;
    int bolidNumber_;
    int timerId_;
    QVector<int> lapsBorders_;
    int classificationPoints_;


signals:
    void bolidDefinition(QVariant,QVariant,QVariant);
    void moveBolid(QVariant,QVariant);
    void newLap(QString, int);
    void crash(QString,QVariant);

public slots:
    void onStartSignal();

};
#endif // BOLID_H
