#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "talk.h"
#include "syncworkserver.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    app.setOrganizationName("Algorithm0");
    app.setOrganizationDomain("github.com/Algorithm0");
    app.setApplicationName("Tonometer");
    app.setApplicationVersion("0.2");

    Talk recipient; //объект взаимодействия с сетью
    SyncWorkServer chain(&app, &recipient, "."); //объект синхронизации с работой сервера
//    if (!recipient.start())
//        return -1;

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    QObject::connect(engine.rootObjects().at(0), SIGNAL(changeParamsNetwork(int,int,QString)),
                     &recipient, SLOT(setNewArgsAndStart(int,int,QString)));
    QObject::connect(engine.rootObjects().at(0), SIGNAL(changePathServer(QString)),
                     &chain, SLOT(setPathToAppDir(QString)));

    QObject::connect(&recipient, SIGNAL(resTalk(QVariant)), engine.rootObjects().at(0)->findChild<QObject*>("winSettings"),
                     SLOT(showWithErrorNetwork(QVariant)));

    QObject::connect(engine.rootObjects().at(0), SIGNAL(startServer()), &chain, SLOT(doSync()));
    QObject::connect(&recipient, SIGNAL(newDataReceived(QVariant,QVariant,QVariant)),
                     engine.rootObjects().at(0), SLOT(newData(QVariant,QVariant,QVariant)));

    auto* const connection = new QMetaObject::Connection;
    *connection = QObject::connect(&recipient, &Talk::newDataReceived,
             [&engine, connection]() {
                  engine.rootObjects().at(0)->findChild<QObject*>("startServerButtion")->setProperty("visible", false);
                  QObject::disconnect(*connection);
                  delete connection;
    }); //"одноразовое" соединение (убрать видимость кнопки запуска сервера при получении первого сообщения)

    return app.exec();
}
