#ifndef BOLID_H
#define BOLID_H

#include "qobject.h"
#include <random>
#include <QStringList>
#include <QColor>

class Bolid : public QObject
{
    Q_OBJECT
public:
    Bolid(QObject *parent = nullptr);

    void configure(const QStringList argv);

private:
    std::random_device rd;
    QString name_ = "F1";
    int start_ = 100;
    int speed_ = 100;
    QColor color_;

signals:
    void bolidDefinition(QString, int, int, QColor);

};

#endif // BOLID_H
