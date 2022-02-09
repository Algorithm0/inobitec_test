#include "talk.h"

#include <QRandomGenerator>
#include <QThread>
#include <QtConcurrent>

Talk::Talk(unsigned char _minHP, unsigned char _maxHP,
           unsigned char _minLP, unsigned char _maxLP,
           unsigned char _minPL, unsigned char _maxPL,
           qint16 port_read, qint16 port_write, qint16 _interval,
           QHostAddress ip) : minHP(_minHP), maxHP(_maxHP), minLP(_minLP), maxLP(_maxLP),
                              minPL(_minPL), maxPL(_maxPL), portW(port_write), portR(port_read),
                              interval(_interval), address(ip)
{
    runFlag.store(false);
    sentMessage.resize(4);
    sentMessage[0] = 'T'; //header bit
    sentMessage[1] = (unsigned char)QRandomGenerator::global()->bounded(minHP, maxHP); //h. pressure
    sentMessage[2] = (unsigned char)QRandomGenerator::global()->bounded(minLP, maxLP); //l. pressure
    sentMessage[3] = (unsigned char)QRandomGenerator::global()->bounded(minPL, maxPL); //pulse
}

Talk::~Talk(){
    if (runFlag.load())
        stop();
    writing.waitForFinished();
}

bool Talk::start()
{
    return connectTcp();
}

void Talk::stop()
{
    disconnect(&socket, SIGNAL(readyRead()),this, SLOT(communicationRead()));
    runFlag.store(false);
    socket.close();
    emit endWork();
}

bool Talk::connectTcp()
{
    if (socket.bind(address, portR))
    {
        connect(&socket, SIGNAL(readyRead()), this, SLOT(communicationRead()));
        runFlag.store(true);
        communicationWrite();
        return true;
    }
    else return false;
}

void Talk::communicationRead()
{
    while (socket.hasPendingDatagrams())
    {
        receivedMessage.resize(socket.pendingDatagramSize());
        socket.readDatagram(receivedMessage.data(), receivedMessage.size(), &sender);
        if (receivedMessage == stopMessage)
        {
            qDebug() << "Stop message received. Completion of the process.";
            stop();
            break;
        }
    }
}

unsigned char randomNewValue(unsigned char max, unsigned char min, unsigned char old, unsigned char max_dif) {
    unsigned char tmp_max, tmp_min;
    tmp_max = old + max_dif > max ? max : old + max_dif;
    tmp_min = old - max_dif < min ? min : old - max_dif;
    return (unsigned char)QRandomGenerator::global()->bounded(tmp_min, tmp_max);
} //генерация нового случайного значения с ограничением по разнице со старым

void Talk::communicationWrite()
{
    writing = QtConcurrent::run([=]()
    {
        socket.writeDatagram(sentMessage, QHostAddress::LocalHost, portW);
        qDebug() << "Send message with " + sentMessage.toHex(' ') + ".";
        QThread::sleep(interval);
        while(runFlag.load())
        {
            sentMessage[1] = randomNewValue(maxHP, minHP, sentMessage[1], 10);
            sentMessage[2] = randomNewValue(maxLP, minLP, sentMessage[2], 10);
            sentMessage[3] = randomNewValue(maxPL, minPL, sentMessage[3], 40);
            socket.writeDatagram(sentMessage, QHostAddress::LocalHost, portW);
            qDebug() << "Send message with " + sentMessage.toHex(' ') + ".";
            QThread::sleep(interval);
        }
    });
}
