#include "talk.h"

#include <QRandomGenerator>
#include <QThread>
#include <QtConcurrent>

Talk::Talk(unsigned char _minHP, unsigned char _maxHP,
           unsigned char _minLP, unsigned char _maxLP,
           unsigned char _minPL, unsigned char _maxPL,
           qint16 port_read, qint16 port_write,
           qint16 _interval) : minHP(_minHP), maxHP(_maxHP), minLP(_minLP), maxLP(_maxLP),
                               minPL(_minPL), maxPL(_maxPL), portW(port_write), portR(port_read),
                               interval(_interval)
{
    run_flag.store(false);
    sent_message.resize(4);
    sent_message[0] = 'T'; //header bit
    sent_message[1] = (unsigned char)QRandomGenerator::global()->bounded(minHP, maxHP); //h. pressure
    sent_message[2] = (unsigned char)QRandomGenerator::global()->bounded(minLP, maxLP); //l. pressure
    sent_message[3] = (unsigned char)QRandomGenerator::global()->bounded(minPL, maxPL); //pulse
}

Talk::~Talk(){
    if (run_flag.load())
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
    run_flag.store(false);
    socket.close();
    emit endWork();
}

bool Talk::connectTcp()
{
    if (socket.bind(QHostAddress::LocalHost, portR))
    {
        connect(&socket, SIGNAL(readyRead()), this, SLOT(communicationRead()));
        run_flag.store(true);
        communicationWrite();
        return true;
    }
    else return false;
}

void Talk::communicationRead()
{
    while (socket.hasPendingDatagrams()) {
        received_message.resize(socket.pendingDatagramSize());
        socket.readDatagram(received_message.data(), received_message.size(), &sender);
        if (received_message == stop_message) {
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
    writing = QtConcurrent::run([=]() {
        socket.writeDatagram(sent_message, QHostAddress::LocalHost, portW);
        qDebug() << "Send message with " + sent_message.toHex(' ') + ".";
        QThread::sleep(interval);
        while(run_flag.load()) {
            sent_message[1] = randomNewValue(maxHP, minHP, sent_message[1], 10);
            sent_message[2] = randomNewValue(maxLP, minLP, sent_message[2], 10);
            sent_message[3] = randomNewValue(maxPL, minPL, sent_message[3], 40);
            socket.writeDatagram(sent_message, QHostAddress::LocalHost, portW);
            qDebug() << "Send message with " + sent_message.toHex(' ') + ".";
            QThread::sleep(interval);
        }
    });
}
