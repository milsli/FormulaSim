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
    void setLaps(int laps);
    int getDistance() const {return distance_;}
    QString getName() {return name_;}

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
    int x_ = 0;
    int y_ = 45;
    int distance_;
    int laps_;
    int currentLap_;
    int bolidNumber_;
    int timerId_;

signals:
    void bolidDefinition(QVariant,QVariant,QVariant);
    void moveBolid(QVariant,QVariant);
    void newLap(QString, int);
    void crash(QString);

public slots:
    void onStartSignal();

};
#endif // BOLID_H