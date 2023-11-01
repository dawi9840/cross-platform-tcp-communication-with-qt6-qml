#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "myclient.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // The QML engine
    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/qml_client/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    // Pass the QML engine to the client
    MyClient client(&engine);
    engine.rootContext()->setContextProperty("myClient", &client);

    engine.load(url);

    // Set the application name.
    app.setApplicationName("My TCP Client");
    return app.exec();
}

