#ifndef SENSORSWIDGET_H
#define SENSORSWIDGET_H

#include <QGroupBox>
#include "socketclient.h"

namespace Ui {
class sensorsWidget;
}

class sensorsWidget : public QGroupBox
{
    Q_OBJECT

public:
    explicit sensorsWidget(QWidget *parent = 0);
    ~sensorsWidget();
    void setClient(socketClient* c)
    {
        m_client = c;
        connect(c,SIGNAL(accRead(int,int,int,int,int,int,float)),this,SLOT(accRead(int,int,int,int,int,int,float)));
    }
private:
    Ui::sensorsWidget   *ui;
    socketClient*       m_client;

protected slots:
    void accRead(int Gx,int Gy,int Gz,int AcX,int AcY,int AcZ,float Temp);
};

#endif // SENSORSWIDGET_H
