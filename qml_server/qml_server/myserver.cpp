#include <QCryptographicHash>
#include <QStandardPaths>
#include <QTcpSocket>
#include <QBuffer>
#include <QImage>
#include <QDebug>
#include "myserver.h"


MyServer::MyServer(QObject *parent): QObject(parent)
{
    // Set necessary initialization in constructor.
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any, 3001))
    {
        qDebug() << "Server could not start!";
    }
    else
    {
        qDebug() << "Server started on port 3001";
        connect(tcpServer, &QTcpServer::newConnection, this, &MyServer::onClientConnected);
    }
}

void MyServer::onClientConnected()
{
    serverSocket = tcpServer->nextPendingConnection();
    qDebug() << "Client connected!";
    // Send a welcome message to the Client.
    serverSocket->write("Hello from the server!");
    connect(serverSocket, &QTcpSocket::readyRead, this, &MyServer::onReadyRead);
}

void MyServer::onReadyRead()
{
    // Handle data from the client if needed
    QByteArray data = serverSocket->readAll();
    if (data == "RequestImageReSend") {
        qDebug() << "Received a request to resend the image.";
        sendImageToClient();
    }
}

void MyServer::sendSongDataToClient(const QString &message)
{
    if (serverSocket && serverSocket->isOpen())
    {
        QByteArray data = message.toUtf8();
        QByteArray newData("S1:");
        newData.append(data);
        newData.append("|");
        serverSocket->write(newData);
        serverSocket->flush();
        //qDebug() << "Song data: " << data;
    } else {
        qDebug() << "serverSocket is not open.";
    }
}

void MyServer::sendArtistDataToClient(const QString &message)
{
    if (serverSocket && serverSocket->isOpen())
    {
        QByteArray artistData = message.toUtf8();
        QByteArray newArtistData("S2:");
        newArtistData.append(artistData);
        newArtistData.append("|");
        serverSocket->write(newArtistData);
        serverSocket->flush();
        //qDebug() << "Artist data: " << newArtistData;
    } else {
        qDebug() << "serverSocket is not open.";
    }
}

void MyServer::sendAlbumDataToClient(const QString &message)
{
    if (serverSocket && serverSocket->isOpen())
    {
        QByteArray albumData = message.toUtf8();
        QByteArray newAlbumData("S3:");
        newAlbumData.append(albumData);
        newAlbumData.append("|");
        serverSocket->write(newAlbumData);
        serverSocket->flush();
        //qDebug() << "Album data: " << newAlbumData;
    } else {
        qDebug() << "serverSocket is not open.";
    }
}

void MyServer::sendCloseCommandToClient()
{
    if (serverSocket && serverSocket->isOpen())
    {
        // Send the closeCommand.
        QByteArray closeCommand = "CloseClient";
        serverSocket->write(closeCommand);
        serverSocket->flush(); // Make sure the information is sent immediately.
        // Disconnect and close the client socket.
        serverSocket->disconnectFromHost();
        serverSocket->close();

    } else {
        qDebug() << "serverSocket is not open.";
    }
}

void MyServer::sendImageToClient()
{
    if (serverSocket && serverSocket->isOpen()) {
        // coverCat100.jpg, coverCP100V2.jpg.
        QImage image(":/images/coverCP100V2.jpg");

        // v1: OK version
        QByteArray newBase64Data = encodeImageToBase64(image);

        serverSocket->write(newBase64Data);
        serverSocket->flush();
        qDebug() << "Image base64 data sent to client.";

        QString imagePath = saveImageToTempLocation(image);
        emit receivedLocoalImagePath(imagePath);
    } else {
        qDebug() << "Client socket is not open.";
    }
}

void MyServer::sendTotalTimeToClient(int totalTime)
{
    if (serverSocket && serverSocket->isOpen())
    {
        QString message = QString::number(totalTime);
        QByteArray data = message.toUtf8();
        QByteArray newData("Time:");
        newData.append(data);
        newData.append("|");
        serverSocket->write(newData);
        serverSocket->flush();
        qDebug() << "Total Time: " << data;
    } else {
        qDebug() << "serverSocket is not open.";
    }
}

QByteArray MyServer::encodeImageToBase64(const QImage &image) {
    // Check if the image is loaded successfully.
    if (image.isNull()) {
        qDebug() << "Failed to load image.";
        return QByteArray(); // Return an empty QByteArray in case of failure.
    }

    // Create a QByteArray to hold Base64 encoded image data.
    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);

    // v2: checksum version
    if (image.save(&buffer, "JPEG")) {
        // Calculate the checksum
        QCryptographicHash checksum(QCryptographicHash::Md5);
        checksum.addData(imageData);
        QByteArray checksumResult = checksum.result();

        // Combine checksum and Base64 data
        QByteArray base64Data = imageData.toBase64();
        QByteArray combinedData("Image:");
        combinedData.append(checksumResult);
        combinedData.append(base64Data);

        return combinedData;
    } else {
        qDebug() << "Failed to save image.";
        return QByteArray();
    }

    // v1: OK version
//    // Write image data to QByteArray in JPEG format
//    image.save(&buffer, "JPEG");

//    // Encode QByteArray to Base64 using toBase64 method
//    QByteArray base64Data = imageData.toBase64();

//    // Create a new QByteArray data, and add the "Image:".
//    QByteArray newBase64Data("Image:");

//    // Add the base64Data into the newBase64Data.
//    newBase64Data.append(base64Data);

//    //qDebug() << "Server base64Data: " << imageData;
//    //qDebug() << "Total imageDatSize: " << newBase64Data.size() << " bytes";
//    qDebug() << "Source imageDatSize: " << imageData.size() << " bytes";
//    return newBase64Data;
}

QString MyServer::saveImageToTempLocation(const QImage &image)
{
    QString tempDir = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QString imagePath = tempDir + "/myimage.jpg";

    // Save the image to the temporary location.
    if (image.save(imagePath, "JPEG")) {
        //qDebug() << "imagePath: " << imagePath;
        return imagePath;
    } else {
        qDebug() << "Return an empty string on failure.";
        return QString();
    }
}

