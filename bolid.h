#ifndef BOLID_H
#define BOLID_H

#include "qobject.h"
#include <QStringList>
#include <QColor>
#include <QVariant>

#define LAP_DISTANCE 3500

class Bolid : public QObject
{
    Q_OBJECT
public:
    static int objectCounter;

    Bolid(QObject *parent = nullptr);

    void configure(const QStringList argv);
    int getCurrentLap() const;
    void setLaps(int laps);
    int getDistance() const {return distance_;}
    QString getName() {return name_;}
    void killBolidTimer();
    int getBolidNumber() const {return bolidNumber_;}

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
    int bolidNumber_;
    int timerId_;
    QVector<int> lapsBorders_;

signals:
    void bolidDefinition(QVariant,QVariant,QVariant);
    void moveBolid(QVariant,QVariant);
    void newLap(QString, int);
    void crash(QString,QVariant);

public slots:
    void onStartSignal();

};
#endif // BOLID_H
