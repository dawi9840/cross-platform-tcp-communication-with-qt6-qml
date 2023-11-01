    #ifndef MYSERVER_H
#define MYSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class MyServer : public QObject
{
    Q_OBJECT

public:
    MyServer(QObject *parent = nullptr);
    Q_INVOKABLE void sendSongDataToClient(const QString &message);
    Q_INVOKABLE void sendArtistDataToClient(const QString &message);
    Q_INVOKABLE void sendAlbumDataToClient(const QString &message);
    Q_INVOKABLE void sendCloseCommandToClient();
    Q_INVOKABLE void sendImageToClient();
    Q_INVOKABLE void sendTotalTimeToClient(int totalTime);
    QByteArray encodeImageToBase64(const QImage &image);
    QString saveImageToTempLocation(const QImage &image);


signals:
    void receivedLocoalImagePath(const QString &imagePath);

private slots:
    void onClientConnected();
    void onReadyRead();

private:
    QTcpServer *tcpServer;
    QTcpSocket *serverSocket;
};

#endif // MYSERVER_H

