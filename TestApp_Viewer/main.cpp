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

    Talk recipient; //объект взаимодействия с сетью
    if (!recipient.start())
        return -1;

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    SyncWorkServer chain(&app, &recipient, "."); //объект синхронизации с работой сервера
    QObject::connect(engine.rootObjects().at(0), SIGNAL(startServer()), &chain, SLOT(doSync()));
    QObject::connect(&recipient, SIGNAL(new_data_received(QVariant,QVariant,QVariant)),
                     engine.rootObjects().at(0), SLOT(newData(QVariant,QVariant,QVariant)));

    QMetaObject::Connection* const connection = new QMetaObject::Connection;
    *connection = QObject::connect(&recipient, &Talk::new_data_received,
             [&engine, connection]() {
                  engine.rootObjects().at(0)->findChild<QObject*>("startServerButtion")->setProperty("visible", false);
                  QObject::disconnect(*connection);
                  delete connection;
    }); //"одноразовое" соединение (убрать видимость кнопки запуска сервера при получении первого сообщения)

    return app.exec();
}
