#include "controlmainwindow.h"
#include "ui_controlmainwindow.h"

controlMainWindow::controlMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::controlMainWindow)
{
    ui->setupUi(this);
    ui->statusBar->addWidget(&m_txWidget);
    connect(&m_socketClient,SIGNAL(tx()),&m_txWidget,SLOT(tx()));
    connect(&m_socketClient,SIGNAL(rx()),&m_txWidget,SLOT(rx()));

    ui->connectionWidget->setClient(&m_socketClient);
    ui->sensorViewWidget->setClient(&m_socketClient);
}

controlMainWindow::~controlMainWindow()
{
    delete ui;
}
