#include "connectwidget.h"
#include "ui_connectwidget.h"

connectWidget::connectWidget(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::connectWidget)
{
    ui->setupUi(this);
    connect(ui->btnConnect,SIGNAL(toggled(bool)),this,SLOT(connectSocket()));
}

connectWidget::~connectWidget()
{
    delete ui;
}

void connectWidget::disconnected()
{
    ui->btnConnect->setChecked(false);
    ui->editIP->show();
    ui->label->show();
    ui->label_2->show();
    ui->editPort->show();
    ui->editIP->setEnabled(true);
    ui->label->setEnabled(true);
    ui->label_2->setEnabled(true);
    ui->editPort->setEnabled(true);
}

void connectWidget::connected()
{
    ui->btnConnect->setChecked(true);
    ui->editIP->hide();
    ui->label->hide();
    ui->label_2->hide();
    ui->editPort->hide();
}

void connectWidget::connectSocket()
{
    if(!ui->btnConnect->isChecked())
        m_client->close();
    else
    {
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
