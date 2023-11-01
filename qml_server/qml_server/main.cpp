#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "myserver.h"
#include "mytimer.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/qml_server/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    MyServer server;
    MyTimer myTimer;

    // Expose server objects to QML.
    engine.rootContext()->setContextProperty("myServer", &server);
    engine.rootContext()->setContextProperty("myTimer", &myTimer);

    // Set string data info to QML.
    QString messageSong = "Left And Right (feat. Jung Kook of BTS)";
    QString messageArtist = "Charlie Puth";
    QString messageAlbum = "Voicenotes";
    engine.rootContext()->setContextProperty("messageSong", messageSong);
    engine.rootContext()->setContextProperty("messageArtist", messageArtist);
    engine.rootContext()->setContextProperty("messageAlbum", messageAlbum);

    // Set the initial total time (3:29) in QML.
    int TotalTime = 209;
    QString convertTotalTime = myTimer.timeToMinutesAndSeconds(TotalTime);
    engine.rootContext()->setContextProperty("TotalTime", TotalTime);
    engine.rootContext()->setContextProperty("convertInitTotalTime", convertTotalTime);

    engine.load(url);

    // Set the application name.
    app.setApplicationName("My TCP Server");
    return app.exec();
}

