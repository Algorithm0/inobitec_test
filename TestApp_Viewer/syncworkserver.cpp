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

void SyncWorkServer::doSync()
{
    if (!isRunning) {
        isRunning = true;
        QString prog_add = serverAppDir + "/TestApp_Server";
        if (QSysInfo::productType() == "windows")
            prog_add += ".exe";
        QFileInfo check_prog(prog_add);
        if (!check_prog.exists() || !check_prog.isFile()) {
            qDebug() << "Server program not found in expected path (" + check_prog.absoluteFilePath() + ')';
            isRunning = false;
            return;
        }
        serverWork = new QProcess(this);
        serverWork->start(prog_add);
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
