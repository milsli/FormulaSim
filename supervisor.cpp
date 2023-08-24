#include "supervisor.h"
#include <QFile>
#include "bolid.h"

Supervisor::Supervisor(QObject *parent)
    : QObject{parent}
    , configPath_("../config/config.cfg")
{
}

Supervisor::~Supervisor()
{
    // delete bolids_
    for(Bolid *bolid : bolids_)
    {
        delete bolid;
    }
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

void Supervisor::processLine(QString &line)
{
    if(line.trimmed()[0] == "#")
        return;

    QStringList configList = line.split(" ");

    if(line.contains("Bolid"))
    {
        Bolid *bolid = new Bolid(this);
        bolid->configure(configList);
        bolids_.push_back(bolid);
        connect(bolid, &Bolid::bolidDefinition, this, &Supervisor::bolidDefinition);
    }
}
