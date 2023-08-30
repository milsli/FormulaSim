#ifndef SUPERVISOR_H
#define SUPERVISOR_H

#include <QObject>
#include <QVariant>


class Bolid;

class Supervisor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QVariant> rankingTable READ rankingTable NOTIFY showRanking)
    Q_PROPERTY(QList<QVariant> crashedCars READ crashedCars NOTIFY crashedCarsSignal)

public:
    explicit Supervisor(QObject *parent = nullptr);
    virtual ~Supervisor();

    void readConfig();

    QList<QVariant> rankingTable();
    QList<QVariant> crashedCars();

private:
    void processLine(QString &line);
    void setRaceConfig(const QStringList conf);

private:
    QString configPath_;
    int currentRaceLap_;
    std::vector<Bolid*> bolids_;
    int laps_;

    QList<QVariant>  result_;
    QList<QVariant>  finalResult_;
    QList<QVariant> crashed_;


signals:
    // void bolidDefinition(QString, int, int, QColor);

//    void showRanking(QVector<QString>);
    void showRanking();
    void crashedCarsSignal();


    void bolidDefinition(QVariant,QVariant,QVariant);
    void startSignal();
    void moveBolid(QVariant,QVariant);
    void newLap(QVariant,QVariant);



private slots:
    void onNewLap(QString name, int currentLap);
    void onMoveBolid(QVariant bolidNumber, QVariant distance);
    void onCrash(QString name);

};
#endif // SUPERVISOR_H
