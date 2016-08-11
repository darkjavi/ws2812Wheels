#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <QObject>
#include <QTcpSocket>

class socketClient : public QTcpSocket
{
    Q_OBJECT
public:
    socketClient();

signals:
    void tx();
    void rx();

    void accRead(int gx,int gy,int gz, int accX,int accY,int accZ,float Temp);
protected slots:
    void readData();
    void sendData(QString data);
public slots:
    void off();
    void setColor(quint8 r, quint8 g, quint8 b);
    void setBrightness(float b);
    void animationCircle();
    void animationDoubleCircle();
    void animationRainbow();
};

#endif // SOCKETCLIENT_H
