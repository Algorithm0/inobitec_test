#include "talk.h"

#include <QRandomGenerator>
#include <QThread>

Talk::Talk(qint16 portRead, qint16 portWrite, QHostAddress ip) : portW(portWrite), portR(portRead), address(ip) {}

Talk::~Talk()
{
    disconnect(&socket, SIGNAL(readyRead()),this, SLOT(communicationRead()));
    socket.close();
}

bool Talk::start()
{
    return connectUdp();
}

void Talk::setNewArgsAndStart(int portRead, int portWrite, QString ip)
{
    if(socket.isValid())
        socket.close();
    QHostAddress tmp(ip);
    if(QAbstractSocket::IPv4Protocol != tmp.protocol())
    {
        emit resTalk(false);
        return;
    }
    address = tmp;
    portR = portRead;
    portW = portWrite;
    connectUdp();
}

bool Talk::connectUdp()
{
    if(socket.bind(address, portR))
    {
        connect(&socket, SIGNAL(readyRead()), this, SLOT(communicationRead()));
        emit resTalk(true);
        return true;
    }
    else
    {
        emit resTalk(false);
        return false;
    }
}

void Talk::communicationRead()
{
    while(socket.hasPendingDatagrams())
    {
        receivedMessage.resize(socket.pendingDatagramSize());
        socket.readDatagram(receivedMessage.data(), receivedMessage.size(), &sender);
        if(receivedMessage.size() > 3 && receivedMessage[0] == 'T')
        {
            emit newDataReceived(QVariant((unsigned char)receivedMessage[1]),
                                   QVariant((unsigned char)receivedMessage[2]), QVariant((unsigned char)receivedMessage[3]));
        }
    }
}

void Talk::signalToStopServer()
{
    if(socket.isValid())
        socket.writeDatagram(stopMessage, address, portW);
}
