#ifndef TALK_H
#define TALK_H

#include <QNetworkDatagram>
#include <QUdpSocket>
#include <QVariant>
#include <atomic>

class Talk final : public QObject
{
    Q_OBJECT

public:
    explicit Talk(qint16 port_read = 20108, qint16 port_write = 8234);
    ~Talk() final;
    bool start();

signals:
    void new_data_received (QVariant h_pre, QVariant l_pre, QVariant pulse);

private:
    QUdpSocket socket;
    QHostAddress sender;
    QByteArray receivedMessage;
    const QByteArray stopMessage = "stop!";
    bool connectUdp();
    qint16 portW, portR;

private slots:
    void communicationRead();
    void signalToStopServer();
};

#endif // TALK_H
