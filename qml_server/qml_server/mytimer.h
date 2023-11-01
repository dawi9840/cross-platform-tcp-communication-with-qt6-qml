#ifndef MYTIMER_H
#define MYTIMER_H

#include <QObject>
#include <QTimer>

class MyTimer : public QObject
{
    Q_OBJECT
public:
    explicit MyTimer(QObject *parent = nullptr);
    QString timeToMinutesAndSeconds(const int &totaltime);

public slots:
    void startCountdown(const int &countdownSeconds);  // Accept countdown seconds.

signals:
    void receivedUpdatedTime(QString time);

private:
    QTimer timer;
    int countdownSeconds;
};

#endif // MYTIMER_H

