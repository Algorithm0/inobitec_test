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
    explicit Talk(qint16 portRead = 20108, qint16 portWrite = 8234, QHostAddress ip = QHostAddress::LocalHost);
    ~Talk() final;
    bool start();
    qint16 getPortR() {return portR;}
    qint16 getPortW() {return portW;}
    QHostAddress getAddress() {return address;}

public slots:
    void setNewArgsAndStart(int portRead = 20108, int portWrite = 8234, QString ip = "127.0.0.1");

signals:
    void newDataReceived(QVariant hPre, QVariant lPre, QVariant pulse);
    void resTalk(QVariant res);

private:
    QUdpSocket socket;
    QHostAddress sender;
    QByteArray receivedMessage;
    const QByteArray stopMessage = "stop!";
    bool connectUdp();
    qint16 portW, portR;
    QHostAddress address;

private slots:
    void communicationRead();
    void signalToStopServer();
};

#endif // TALK_H
