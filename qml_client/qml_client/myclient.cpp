#include <QQmlApplicationEngine>
#include <QCryptographicHash>
#include <QCoreApplication>
#include <QStandardPaths>
#include <QQmlContext>
#include <QDebug>
#include <QImage>
#include "mytimer.h"
#include "myclient.h"


MyClient::MyClient(QQmlApplicationEngine *qmlEngine, QObject *parent)
    : QTcpSocket(parent), qmlEngine(qmlEngine)
{
    connect(this, &QTcpSocket::readyRead, this, &MyClient::onReadyRead);

    // Connect to the Server
    connectToHost("127.0.0.1", 3001);        // ubuntu_server, ubuntu_client
    //connectToHost("192.168.182.192", 3001);  // Android13_emulator_server, qnx_client.
    //connectTo Host("192.168.183.140", 3001); // qnx_server, ubuntu_client.
    //connectToHost("10.74.198.158", 3001);    // phone_server, qnx_client/ubuntu_client; Check phone IP.

    myTimerObject = new MyTimer(this);
    QQmlContext *qmlContext = qmlEngine->rootContext(); // Set myTimerObject as a QML context property.
    qmlContext->setContextProperty("myTimerObject", myTimerObject);
}

void MyClient::onReadyRead()
{
    QByteArray data = readAll();
    if(data == "CloseClient") {
        qDebug() << "Received close command from server.";
        if (state() == QAbstractSocket::ConnectedState) {
            qDebug() << "Close the socket connect and clos the client program.";
            disconnectFromHost();      // Disconnect only if connected
            QCoreApplication::quit();  // Quit the application
        }
    } else if (data.startsWith("Image:")) {
        qDebug() << "Received image data.";
        QImage image = decodeBase64ToImage(data);
        if (!image.isNull()) {
            // Image and checksum are valid
            QString imagePath = saveImageToTempLocation(image);
            emit receivedImageDataPath(imagePath);
        } else {
            // Checksum mismatch or image is invalid
            qDebug() << "The checksums do not match and the image data may be corrupted.";
            // Handle the case when the checksum doesn't match or the image is invalid
            requestImageReSend();
        }
    } else {
        // Process other string data received from the server.
        //qDebug() << "Received data:" << data;
        char separator = '|';
        QList<QByteArray> messages = data.split(separator);
        foreach (const QByteArray &message, messages) {
            if (message.startsWith("S1:")) {
                QByteArray songData = message.mid(3);
                emit receivedSongData(QString(songData));
            } else if (message.startsWith("S2:")) {
                QByteArray artistData = message.mid(3);
                emit receivedArtistData(QString(artistData));
            } else if (message.startsWith("S3:")) {
                QByteArray albumData = message.mid(3);
                emit receivedAlbumData(QString(albumData));
            }else if (message.startsWith("Time:")) {
                QByteArray totalTimeData = message.mid(5);

                // Convert QByteArray to QString.
                QString stringTotalTime = QString::fromUtf8(totalTimeData);

                // Convert QString to integer.
                bool ok;
                int intTotalTime = stringTotalTime.toInt(&ok);
                //qDebug() << "intTotalTime: " << intTotalTime; // 209

                QString newTotalTime = myTimerObject->timeToMinutesAndSeconds(intTotalTime);
                //qDebug() << "newTotalTime: " << newTotalTime; //3:29

                emit receivedInitTotalTime(newTotalTime);
                myTimerObject->startCountdown(intTotalTime);
            }
        }
    }
}

void MyClient::requestImageReSend()
{
    if (state() == QAbstractSocket::ConnectedState) {
        // Send a request telling the server to resend the image.
        write("RequestImageReSend");
        flush();
        qDebug() << "RequestImageReSend------->";
    } else {
        qDebug() << "The socket is not open.";
    }
}

QImage MyClient::decodeBase64ToImage(const QByteArray &newBase64Data)
{
    // Verify the format and length of the received data
    if (newBase64Data.size() < 22 || !newBase64Data.startsWith("Image:")) {
        qDebug() << "Invalid image data format.";
        return QImage();
    }

    // Extract checksum and Base64 data
    QByteArray receivedChecksum = newBase64Data.mid(6, 16);
    QByteArray base64Data = newBase64Data.mid(22);

    // Decode Base64 data to raw image data
    QByteArray imageData = QByteArray::fromBase64(base64Data);

    // Calculate the checksum for the received data
    QCryptographicHash checksum(QCryptographicHash::Md5);
    checksum.addData(imageData);
    QByteArray computedChecksum = checksum.result();

    // Compare the computed checksum with the received checksum
    if (computedChecksum != receivedChecksum) {
        qDebug() << "Checksum mismatch, image data may be corrupted.";
        return QImage();
    }

    // Create a QImage from the raw image data
    QImage image;
    if (image.loadFromData(imageData, "JPEG")) {
        return image;
    } else {
        qDebug() << "Failed to load image from decoded data.";
        return QImage();
    }
}

QString MyClient::saveImageToTempLocation(const QImage &image)
{
    QString tempDir = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QString imagePath = tempDir + "/myimage.jpg";

    // Save the image to the temporary location.
    if (image.save(imagePath, "JPEG")) {
        //qDebug() << "imagePath: " << imagePath;
        return imagePath;
    } else {
        return QString(); // Return an empty string on failure.
    }
}

