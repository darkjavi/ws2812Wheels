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

    connect(ui->connectionWidget,SIGNAL(ready())    ,this,SLOT(enableUI()));
    connect(ui->connectionWidget,SIGNAL(notReady()) ,this,SLOT(disableUI()));

    ui->connectionWidget->setClient(&m_socketClient);
    ui->sensorViewWidget->setClient(&m_socketClient);

    connect(ui->btnOff,           SIGNAL(clicked(bool)),&m_socketClient,  SLOT(off()));
    connect(ui->btnCircle,        SIGNAL(clicked(bool)),&m_socketClient,  SLOT(animationCircle()));
    connect(ui->btnDoubleCircle,  SIGNAL(clicked(bool)),&m_socketClient,  SLOT(animationDoubleCircle()));
    connect(ui->btnRainbow,       SIGNAL(clicked(bool)),&m_socketClient,  SLOT(animationRainbow()));
    connect(ui->btnSetColor,      SIGNAL(clicked(bool)),this,             SLOT(setColor()));
    connect(ui->sliderBrightness, SIGNAL(valueChanged(int)),this,         SLOT(setBrightness(int)));

    disableUI();
}

controlMainWindow::~controlMainWindow()
{
    delete ui;
}

void controlMainWindow::setBrightness(int value)
{
    m_socketClient.setBrightness(value/100.0);
}

void controlMainWindow::setColor()
{
    m_socketClient.setColor(128,128,0);
}

void controlMainWindow::enableUI()
{
    bool state = true;
    ui->sensorViewWidget->setEnabled(state);
    ui->groupBrightness->setEnabled(state);
    ui->effectsWidget->setEnabled(state);
}

void controlMainWindow::disableUI()
{
    bool state = false;
    ui->sensorViewWidget->setEnabled(state);
    ui->groupBrightness->setEnabled(state);
    ui->effectsWidget->setEnabled(state);
}
