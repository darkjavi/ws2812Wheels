#ifndef SENSORSWIDGET_H
#define SENSORSWIDGET_H

#include <QGroupBox>
#include "socketclient.h"
#include "gsimplegraph.h"

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
        connect(c,SIGNAL(sensorRead(int,int,int,int,int,int,float,int)),this,SLOT(sensorRead(int,int,int,int,int,int,float,int)));
    }
private:
    Ui::sensorsWidget   *ui;
    socketClient*       m_client;
    gSimpleGraph*       m_graph_gx;
    gSimpleGraph*       m_graph_gy;
    gSimpleGraph*       m_graph_gz;
    gSimpleGraph*       m_graph_accx;
    gSimpleGraph*       m_graph_accy;
    gSimpleGraph*       m_graph_accz;

    void addGraphs();

protected slots:
    void sensorRead(int Gx, int Gy, int Gz, int AcX, int AcY, int AcZ, float Temp, int vuLevel);
};

#endif // SENSORSWIDGET_H
