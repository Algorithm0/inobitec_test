#include "talk.h"

#include <QRandomGenerator>
#include <QThread>

Talk::Talk(qint16 port_read, qint16 port_write) : portW(port_write), portR(port_read) {}

Talk::~Talk()
{
    disconnect(&socket, SIGNAL(readyRead()),this, SLOT(communicationRead()));
    socket.close();
}

bool Talk::start()
{
    return connectUdp();
}

bool Talk::connectUdp()
{
    if (socket.bind(QHostAddress::LocalHost, portR))
    {
        connect(&socket, SIGNAL(readyRead()), this, SLOT(communicationRead()));
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
        if (receivedMessage.size() > 3 && receivedMessage[0] == 'T')
        {
            emit new_data_received(QVariant((unsigned char)receivedMessage[1]),
                                   QVariant((unsigned char)receivedMessage[2]), QVariant((unsigned char)receivedMessage[3]));
        }
    }
}

void Talk::signalToStopServer()
{
    socket.writeDatagram(stopMessage, QHostAddress::LocalHost, portW);
}
