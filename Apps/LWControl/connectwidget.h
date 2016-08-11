#ifndef CONNECTWIDGET_H
#define CONNECTWIDGET_H

#include <QGroupBox>
#include "socketclient.h"

namespace Ui {
class connectWidget;
}

class connectWidget : public QGroupBox
{
    Q_OBJECT

public:
    explicit connectWidget(QWidget *parent = 0);
    ~connectWidget();
    void setClient(socketClient* c)
    {
        m_client = c;
        connect(c,SIGNAL(disconnected()),this,SLOT(disconnected()));
        connect(c,SIGNAL(connected()),this,SLOT(connected()));
        connect(c,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(socketError(QAbstractSocket::SocketError)));
    }

private:
    Ui::connectWidget   *ui;
    socketClient*       m_client;

signals:
    void ready();
    void notReady();

protected slots:
    void disconnected();
    void connected();
    void connectSocket();
    void socketError(QAbstractSocket::SocketError);
};

#endif // CONNECTWIDGET_H
