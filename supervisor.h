#ifndef SUPERVISOR_H
#define SUPERVISOR_H

#include <QObject>
#include <QVariant>
#include <qqml.h>

class Bolid;

class CrashedCars : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameSignal)
    Q_PROPERTY(int number READ number NOTIFY numberSignal )
    QML_ANONYMOUS

public:
    Q_INVOKABLE CrashedCars(QObject *parent = nullptr) : QObject(parent)  {}
    CrashedCars(const QString name, const int number, QObject *parent = nullptr) : QObject(parent)
    {
        name_ = name;
        number_ = number;
    }

    ~CrashedCars(){}
    QString name() const {return name_;}
    int number () const {return number_;}

    QString name_;
    int number_;

signals:
    void nameSignal();
    void numberSignal();
};

class Supervisor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QVariant> rankingTable READ rankingTable NOTIFY showRanking)
    Q_PROPERTY(QQmlListProperty<CrashedCars> crashedCars READ crashedCars NOTIFY crashedCarsSignal)

//    Q_PROPERTY(QQmlListProperty<CrashedCars> crashedCarsBis READ crashedCarsBis NOTIFY crashedCarsSignalBis)

public:
    explicit Supervisor(QObject *parent = nullptr);
    virtual ~Supervisor();

    void readConfig();

    QList<QVariant> rankingTable();
    QQmlListProperty<CrashedCars> crashedCars();
    QQmlListProperty<CrashedCars> crashedCarsBis();

private:
    void processLine(QString &line);
    void setRaceConfig(const QStringList conf);

private:
    QString configPath_;
    int currentRaceLap_;
    std::vector<Bolid*> bolids_;
    int laps_;

    QList<QVariant>  result_;
    QList<CrashedCars*> crashed_;

   // QList<CrashedCars*> crashedCarsBB;


signals:
    void showRanking();
    void crashedCarsSignal(QVariant);
//    void crashedCarsSignalBis();

    void bolidDefinition(QVariant,QVariant,QVariant);
    void startSignal();
    void moveBolid(QVariant,QVariant);
    void newLap(QVariant,QVariant);


private slots:
    void onNewLap(QString name, int currentLap);
    void onMoveBolid(QVariant bolidNumber, QVariant distance);
    void onCrash(QString name, QVariant bolidNumber);
    void onNewRace();

};
#endif // SUPERVISOR_H
