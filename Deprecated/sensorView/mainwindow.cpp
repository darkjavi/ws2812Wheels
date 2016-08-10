#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qdebug.h>
#include <qstringlist.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&m_serial,SIGNAL(readyRead()),this,SLOT(readSerial()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readSerial(){
    m_mutex.lock();
    m_buffer.append(m_serial.readAll());
    m_mutex.unlock();
    parseRxBuffer();
}

void MainWindow::parseRxBuffer() {
    m_mutex.lock();
    QStringList list;
    list = m_buffer.split("\n");
    if(list.last().isNull()) list.removeLast();
    for(int i = 0 ; i < list.count() ; i++)
    {
        QString cmd = list.at(i);
        if(cmd.startsWith("v:")) {
            int value = cmd.split(":").at(1).toInt();
            if(value < vmin)
            {
                vmin = value;
                ui->pbs->setMinimum(value);
            } else if (value > vmax)
            {
                vmax = value;
                ui->pbs->setMaximum(value);
            }
            ui->pbs->setValue(value);
        }else if(cmd.startsWith("ax:")) {
            int value = cmd.split(":").at(1).toInt();
            if(value < axmin)
            {
                axmin = value;
                ui->pbx->setMinimum(value);
            } else if (value > axmax)
            {
                axmax = value;
                ui->pbx->setMaximum(value);
            }
            ui->pbx->setValue(value);
        }else if(cmd.startsWith("ay:")) {
            int value = cmd.split(":").at(1).toInt();
            if(value < aymin)
            {
                aymin = value;
                ui->pby->setMinimum(value);
            } else if (value > aymax)
            {
                aymax = value;
                ui->pby->setMaximum(value);
            }
            ui->pby->setValue(value);
        }else if(cmd.startsWith("az:")) {
            int value = cmd.split(":").at(1).toInt();
            if(value < azmin)
            {
                azmin = value;
                ui->pbz->setMinimum(value);
            } else if (value > azmax)
            {
                azmax = value;
                ui->pbz->setMaximum(value);
            }
            ui->pbz->setValue(value);
        }else if(cmd.startsWith("gx:")) {
            int value = cmd.split(":").at(1).toInt();
            if(value < gxmin)
            {
                gxmin = value;
                ui->dx->setMinimum(value);
                ui->xmin->setText(QString::number(value));
            } else if (value > gxmax)
            {
                gxmax = value;
                ui->dx->setMaximum(value);
                ui->xmax->setText(QString::number(value));
            }
            ui->dx->setValue(value);
        }else if(cmd.startsWith("gy:")) {
            int value = cmd.split(":").at(1).toInt();
            if(value < gymin)
            {
                gymin = value;
                ui->dy->setMinimum(value);
                ui->ymin->setText(QString::number(value));
            } else if (value > gymax)
            {
                gymax = value;
                ui->dy->setMaximum(value);
                ui->ymax->setText(QString::number(value));
            }
            ui->dy->setValue(value);
        }else if(cmd.startsWith("gz:")) {
            int value = cmd.split(":").at(1).toInt();
            if(value < gzmin)
            {
                gzmin = value;
                ui->dz->setMinimum(value);
            } else if (value > gzmax)
            {
                gzmax = value;
                ui->dz->setMaximum(value);
            }
            ui->dz->setValue(value);
        }
    }
    m_buffer.clear(); // alomejor se pierde el ultimo comando
    m_mutex.unlock();
}

void MainWindow::on_connectBtn_clicked(){
    m_serial.close();
    m_serial.setPortName(this->ui->portEdit->text());
    m_serial.setBaudRate(115200);
    m_serial.open(QIODevice::ReadOnly);
}
