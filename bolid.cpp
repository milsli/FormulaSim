#include "bolid.h"

Bolid::Bolid(QObject *parent) : QObject(parent)
{
}

void Bolid::configure(const QStringList argv)
{
    for(int i = 0; i < argv.size(); ++i)
    {
        if(argv.contains("-driver")) {
            name_ = argv[i + 1];
        }
        if(argv.contains("-start")) {
            start_ = argv[i + 1].toInt();
        }
        if(argv.contains("-speed")) {
            speed_ = argv[i + 1].toInt();
        }
        if(argv.contains("-color")) {
            bool b;
            color_ = argv[i + 1].toInt(&b,16);
        }
    }

    emit bolidDefinition(name_, start_, speed_, color_);
}
