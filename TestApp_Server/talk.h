#ifndef TALK_H
#define TALK_H

#include <QNetworkDatagram>
#include <QUdpSocket>
#include <QFuture>
#include <atomic>

class Talk : public QObject
{
    Q_OBJECT

public:
    Talk(unsigned char _minHP, unsigned char _maxHP,
         unsigned char _minLP, unsigned char _maxLP,
         unsigned char _minPL, unsigned char _maxPL,
         qint16 port_read = 8234, qint16 port_write = 20108,
         qint16 _interval = 1);
    ~Talk();
    bool start();
    void stop();

signals:
    void endWork();

private:
    QUdpSocket socket;
    QHostAddress sender;
    unsigned char minHP, maxHP, minLP, maxLP, minPL, maxPL;
    QByteArray sent_message, sent_messageOld, received_message;
    const QByteArray stop_message = "stop!";
    bool connectTcp();
    qint16 portW, portR, interval;
    std::atomic<bool> run_flag;
    QFuture<void> writing;
    void communicationWrite();


private slots:
    void communicationRead();
};

#endif // TALK_H
