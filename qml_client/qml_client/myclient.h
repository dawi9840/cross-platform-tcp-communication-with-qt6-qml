#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QQmlApplicationEngine>
#include <QTcpSocket>
#include <QObject>

class MyTimer; // Forward declaration of MyTimer class

class MyClient : public QTcpSocket
{
    Q_OBJECT

public:
    MyClient(QQmlApplicationEngine *qmlEngine, QObject *parent = nullptr);
    QImage decodeBase64ToImage(const QByteArray &newBase64Data);
    QString saveImageToTempLocation(const QImage &image);

public slots:
    void requestImageReSend();

signals:
    void receivedSongData(const QString &data);
    void receivedArtistData(const QString &data);
    void receivedAlbumData(const QString &data);
    void receivedImageDataPath(const QString &imagePath);
    void receivedInitTotalTime(const QString &totalTimeData);

private slots:
    void onReadyRead();

private:
    QQmlApplicationEngine *qmlEngine;
    MyTimer *myTimerObject; // Declare the MyTimer pointer
};

#endif // MYCLIENT_H

