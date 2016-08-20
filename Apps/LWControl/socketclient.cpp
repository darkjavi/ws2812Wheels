#include "socketclient.h"
#include "qdebug.h"

#include "qstringlist.h"

socketClient::socketClient()
{
    connect(this,SIGNAL(readyRead()),this,SLOT(readData()));
}

void socketClient::readData()
{
    emit rx();
    QString data = this->readAll().trimmed();
    //qDebug() << "DATA READ:" << data;
    if(data.startsWith("AcX"))
    {
        QStringList fields = data.split("|");
        if(fields.count() != 8)
        {
            qDebug() << "Unexpected number of fields to parse";
            qDebug() << fields;
            return;
        }

        int vuLevel;
        float AcX,AcY,AcZ,Gx,Gy,Gz,angleY,rawAngleY,Temp;
        AcX     =   fields[0].split(":")[1].toFloat();
        AcY     =   fields[1].split(":")[1].toFloat();
        AcZ     =   fields[2].split(":")[1].toFloat();
        Gx      =   fields[3].split(":")[1].toFloat();
        Gy      =   fields[4].split(":")[1].toFloat();
        Gz      =   fields[5].split(":")[1].toFloat();
        angleY    =   fields[6].split(":")[1].toFloat();
        rawAngleY =   fields[7].split(":")[1].toFloat();
        Temp    =   fields[8].split(":")[1].toFloat();
        vuLevel =   fields[9].split(":")[1].toInt();
        emit sensorRead(Gx,Gy,Gz,AcX,AcY,AcZ,angleY,rawAngleY,Temp,vuLevel);
    }
    else
    {
        qDebug() << "Undefined msg: " << data;
    }

}

void socketClient::sendData(QString data)
{
    emit tx();
    data.append("\n");
    write(data.toUtf8());
}



void socketClient::off()
{
    sendData("off");
}

void socketClient::setColor(quint8 r, quint8 g, quint8 b)
{
    QString data;
    data += "setColor|";
    data += QString::number(r);
    data += "|";
    data += QString::number(g);
    data += "|";
    data += QString::number(b);

    sendData(data);
}

void socketClient::setBrightness(float b)
{
    QString data;
    data += "setBrightness|";
    data += QString::number(b,'f',2);
    sendData(data);
}

void socketClient::nextEffect()
{
    sendData("nextEffect");
}

void socketClient::prevEffect()
{
    sendData("prevEffect");
}


void socketClient::animationCircle()
{
    sendData("animationCircle");
}

void socketClient::animationDoubleCircle()
{
   sendData("animationDoubleCircle");
}

void socketClient::animationRainbow()
{
   sendData("animationRainbow");
}

void socketClient::animationFlash()
{
    sendData("animationFlash");
}

void socketClient::setOffset(int value)
{
    QString str;
    str.append("setOffset|");
    str.append(QString::number(value));
    sendData(str);
}

void socketClient::requestSensorData(bool value)
{
    if(value)
        sendData("sendSensorsData");
    else
    {
        sendData("dontSendSensorsData");
    }
}
