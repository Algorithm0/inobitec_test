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
    while (socket.hasPendingDatagrams()) {
        received_message.resize(socket.pendingDatagramSize());
        socket.readDatagram(received_message.data(), received_message.size(), &sender);
        if (received_message.size() > 3 && received_message[0] == 'T') {
            emit new_data_received(QVariant((unsigned char)received_message[1]),
                    QVariant((unsigned char)received_message[2]), QVariant((unsigned char)received_message[3]));
        }
    }
}

void Talk::signalToStopServer()
{
    socket.writeDatagram(stop_message, QHostAddress::LocalHost, portW);
}
