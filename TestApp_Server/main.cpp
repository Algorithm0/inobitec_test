#include <QCoreApplication>
#include "talk.h"

const unsigned char minHP = 100;
const unsigned char maxHP = 200;
const unsigned char minLP = 70;
const unsigned char maxLP = 110;
const unsigned char minPL = 40;
const unsigned char maxPL = 200;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "Start device emulator.";

    Talk medical_device(minHP, maxHP, minLP, maxLP, minPL, maxPL); //создание объекта для эмуляции сети с ограничениями по случайным данным

    if (!medical_device.start())
        return -1;

    QObject::connect(&medical_device, SIGNAL(endWork()), &a, SLOT(quit()));

    return a.exec();
}
