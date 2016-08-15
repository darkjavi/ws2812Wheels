#include "connectwidget.h"
#include "ui_connectwidget.h"

connectWidget::connectWidget(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::connectWidget)
{
    ui->setupUi(this);
}

connectWidget::~connectWidget()
{
    delete ui;
}

void connectWidget::disconnected()
{
    this->show();
    ui->editIP->setEnabled(true);
    ui->label->setEnabled(true);
    ui->label_2->setEnabled(true);
    ui->editPort->setEnabled(true);
    emit notReady();
}

void connectWidget::connected()
{
    this->hide();
    emit ready();
}

void connectWidget::connectSocket(bool state)
{
    if(!state)
        m_client->close();
    else
    {
        m_client->close();
        m_client->connectToHost(ui->editIP->text(),ui->editPort->text().toInt());
        ui->editIP->setEnabled(false);
        ui->label->setEnabled(false);
        ui->label_2->setEnabled(false);
        ui->editPort->setEnabled(false);
    }
}

void connectWidget::socketError(QAbstractSocket::SocketError e)
{
    qDebug() << "Socket Error" << e;
    m_client->close();
    disconnected();
}
