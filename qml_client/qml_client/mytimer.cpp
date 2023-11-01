#include <QDebug>
#include "mytimer.h"


MyTimer::MyTimer(QObject *parent) : QObject(parent), countdownSeconds(0)
{
    // Initialized to countdownSeconds(0), the countdown needs to be started externally.
    connect(&timer, &QTimer::timeout, this, [this]() {
        if (countdownSeconds >= 0) {
            QString timeString = timeToMinutesAndSeconds(countdownSeconds);

            emit receivedUpdatedTime(timeString);
            //qDebug() << "Received updated time: " << timeString;

            countdownSeconds--;
        } else {
            timer.stop();
        }
    });
}

void MyTimer::startCountdown(const int &countdownSeconds)
{
    this->countdownSeconds = countdownSeconds;
    timer.start(1000); // Updated per second.
}

QString MyTimer::timeToMinutesAndSeconds(const int &totaltime)
{
    int minutes = totaltime/ 60;
    int seconds = totaltime % 60;
    QString convertTotalTime = QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
    return convertTotalTime;
}

