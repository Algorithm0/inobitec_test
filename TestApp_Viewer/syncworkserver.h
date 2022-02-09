#ifndef SYNCWORKSERVER_H
#define SYNCWORKSERVER_H

#include <QObject>
#include <QProcess>
#include <QCoreApplication>
#include "talk.h"

class SyncWorkServer final : public QObject
{
    Q_OBJECT

private:
    QProcess* serverWork = nullptr;
    QCoreApplication *parent;
    Talk *accomplice;
    QString serverAppDir;
    bool isRunning = false;

public:
    explicit SyncWorkServer(QCoreApplication *_parent, Talk *_accomplice, QString _serverAppDir = "C:/server");
    ~SyncWorkServer() final;
    [[maybe_unused]] [[nodiscard]] bool runningStatus() const { return isRunning; }

public slots:
    void doSync(); //запуск синхронизации (и непосредственный запуск программы-сервера)

private slots:
    void endServerApp();

};

#endif // SYNCWORKSERVER_H
