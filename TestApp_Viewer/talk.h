#ifndef TALK_H
#define TALK_H

#include <QNetworkDatagram>
#include <QUdpSocket>
#include <QVariant>
#include <atomic>

class Talk : public QObject
{
    Q_OBJECT

public:
    Talk(qint16 port_read = 20108, qint16 port_write = 8234);
    ~Talk();
    bool start();

signals:
    void new_data_received (QVariant h_pre, QVariant l_pre, QVariant pulse);

private:
    QUdpSocket socket;
    QHostAddress sender;
    QByteArray received_message;
    const QByteArray stop_message = "stop!";
    bool connectUdp();
    qint16 portW, portR;

private slots:
    void communicationRead();
    void signalToStopServer();
};

#endif // TALK_H
