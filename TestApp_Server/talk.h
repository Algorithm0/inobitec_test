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
         qint16 _interval = 1, QHostAddress ip = QHostAddress::LocalHost);
    ~Talk() override;
    bool start();
    void stop();

signals:
    void endWork();

private:
    QUdpSocket socket;
    QHostAddress sender;
    unsigned char minHP, maxHP, minLP, maxLP, minPL, maxPL;
    QByteArray sentMessage, receivedMessage;
    const QByteArray stopMessage = "stop!";
    bool connectTcp();
    qint16 portW, portR, interval;
    std::atomic<bool> runFlag;
    QFuture<void> writing;
    void communicationWrite();
    QHostAddress address;


private slots:
    void communicationRead();
};

#endif // TALK_H
