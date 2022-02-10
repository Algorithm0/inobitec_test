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

    qint16 port_read = 8234;
    qint16 port_write = 20108;
    qint16 interval = 1;
    QHostAddress ip("127.0.0.1");

    int counter = 0;
    if(argc > 1)
    {
        if(!strcmp(argv[1], (char*)"-help"))
        {
            qDebug() << "You can use the following command line arguments:";
            qDebug() << "'-ip%IP' to link to an address,";
            qDebug() << "'-portR%port' & '-portW%port' to specify the corresponding ports,";
            qDebug() << "'-interval%i' to specify the interval for sending messages,";
            qDebug() << "'-help' for help.";
            qDebug() << "Arguments and keys are entered without quotes.";
            qDebug();
            qDebug() << "If you do not specify any of the above, the following values will be used:";
            qDebug() << "Port for read/write is " + QString::number(port_read) + "/" + QString::number(port_write) +
                        ", address is " + ip.toString() + " and interval is " + QString::number(interval);
            return 1;
        }
        QVector<char*> myKeys(4);
        myKeys[0] = (char*)"-ip";
        myKeys[1] = (char*)"-portR";
        myKeys[2] = (char*)"-portW";
        myKeys[3] = (char*)"-interval";
        int j = 0;
        char* argWithoutKey;
        int argValue;
        QHostAddress tmpIp;
        for(int i = 1; i < argc && counter < myKeys.size(); i++)
        {
            for(j = 0; j < myKeys.size() && counter < myKeys.size(); j++)
            {
                argWithoutKey = strstr(argv[i], myKeys[j]);
                if(argWithoutKey && !strcmp(argWithoutKey, argv[i]))
                {
                    switch (j)
                    {
                    case 0:
                    {
                        argWithoutKey+=3;
                        tmpIp = QHostAddress(argWithoutKey);
                        if (QAbstractSocket::IPv4Protocol != tmpIp.protocol())
                        {
                           qDebug("Not valid IPv4 address in key '-ip'. Stop.");
                           std::exit(-1);
                        }
                        ip=tmpIp;
                        counter++;
                        break;
                    }
                    case 1:
                    {
                        argWithoutKey+=6;
                        argValue = strtol(argWithoutKey, nullptr, 10);
                        if(!argValue)
                        {
                            qDebug("Not valid port for read in key '-portR'. Stop.");
                            std::exit(-2);
                        }
                        port_read = argValue;
                        counter++;
                        break;
                    }
                    case 2:
                    {
                        argWithoutKey+=6;
                        argValue = strtol(argWithoutKey, nullptr, 10);
                        if(!argValue)
                        {
                            qDebug("Not valid port for read in key '-portW'. Stop.");
                            std::exit(-3);
                        }
                        port_write = argValue;
                        counter++;
                        break;
                    }
                    case 3:
                    {
                        argWithoutKey+=9;
                        argValue = strtol(argWithoutKey, nullptr, 10);
                        if(!argValue)
                        {
                            qDebug("Not valid port for read in key '-interval'. Stop.");
                            std::exit(-4);
                        }
                        interval = argValue;
                        counter++;
                        break;
                    }
                    }
                }
            }
        }
    }

    qDebug() << "Start device emulator.";

    Talk medical_device(minHP, maxHP, minLP, maxLP, minPL, maxPL,
                        port_read, port_write, interval); //создание объекта для эмуляции сети с ограничениями по случайным данным

    if(!medical_device.start())
        return -400;

    QObject::connect(&medical_device, SIGNAL(endWork()), &a, SLOT(quit()));

    return a.exec();
}
