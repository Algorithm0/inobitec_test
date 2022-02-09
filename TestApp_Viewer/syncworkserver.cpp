#include "syncworkserver.h"
#include <QFileInfo>

SyncWorkServer::SyncWorkServer(QCoreApplication *_parent, Talk *_accomplice, QString _serverAppDir)
    : QObject(_parent), parent(_parent), accomplice(_accomplice), serverAppDir(_serverAppDir) {}

SyncWorkServer::~SyncWorkServer()
{
    if (serverWork != nullptr)
    {
        serverWork->kill();
        delete serverWork;
        isRunning = false;
    }
    disconnect(parent, SIGNAL(aboutToQuit()), accomplice, SLOT(signalToStopServer()));
    disconnect(parent, SIGNAL(aboutToQuit()), this, SLOT(endServerApp()));
}

void SyncWorkServer::setPathToAppDir(QString path)
{
    serverAppDir = path;
    if (isRunning)
    {
        serverWork->kill();
        endServerApp();
        doSync();
    }
}

void SyncWorkServer::doSync()
{
    if (!isRunning)
    {
        isRunning = true;
        QString programAddress = serverAppDir + "/TestApp_Server";
        if (QSysInfo::productType() == "windows")
            programAddress += ".exe";
        QFileInfo programFile(programAddress);
        if (!programFile.exists() || !programFile.isFile())
        {
            qDebug() << "Server program not found in expected path (" + programFile.absoluteFilePath() + ')';
            isRunning = false;
            return;
        }
        QStringList arguments;
        arguments << "-ip" + accomplice->getAddress().toString()
                  << "-portW" + QString(accomplice->getPortR())
                  << "-portR" + QString(accomplice->getPortW());
        serverWork = new QProcess(this);
        serverWork->start(programAddress, arguments);
        connect(parent, SIGNAL(aboutToQuit()), accomplice, SLOT(signalToStopServer()));
        connect(parent, SIGNAL(aboutToQuit()), this, SLOT(endServerApp()));
    }
}

void SyncWorkServer::endServerApp()
{
    serverWork->waitForFinished();
    isRunning = false;
    delete serverWork;
    serverWork = nullptr;
}
