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

};

#endif // SOCKETCLIENT_H
