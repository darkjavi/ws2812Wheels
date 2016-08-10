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
        if(fields.count() != 7)
        {
            qDebug() << "Unexpected number of fields to parse";
            qDebug() << fields;
            return;
        }

        int AcX,AcY,AcZ,Gx,Gy,Gz;
        float Temp;
        AcX =   fields[0].split(":")[1].toInt();
        AcY =   fields[1].split(":")[1].toInt();
        AcZ =   fields[2].split(":")[1].toInt();
        Gx =    fields[3].split(":")[1].toInt();
        Gy =    fields[4].split(":")[1].toInt();
        Gz =    fields[5].split(":")[1].toInt();
        Temp =  fields[6].split(":")[1].toFloat();
        emit accRead(Gx,Gy,Gz,AcX,AcY,AcZ,Temp);
    }
    else
    {
        qDebug() << "Undefined msg: " << data;
    }

}

void socketClient::sendData(QString data)
{
    emit tx();
    write(data.toUtf8());
}
