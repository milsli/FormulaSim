#ifndef SUPERVISOR_H
#define SUPERVISOR_H

#include <QObject>

class Bolid;

class Supervisor : public QObject
{
    Q_OBJECT
public:
    explicit Supervisor(QObject *parent = nullptr);
    virtual ~Supervisor();

    void readConfig();

private:
    void processLine(QString &line);

private:
    QString configPath_;
    std::vector<Bolid*> bolids_;


signals:
    void bolidDefinition(QString, int, int, QColor);

};

#endif // SUPERVISOR_H
