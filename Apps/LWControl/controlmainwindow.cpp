#include "controlmainwindow.h"
#include "ui_controlmainwindow.h"

#include <QToolButton>

controlMainWindow::controlMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::controlMainWindow)
{
    ui->setupUi(this);
    ui->statusBar->addWidget(&m_txWidget);
    connect(&m_socketClient,SIGNAL(tx()),&m_txWidget,SLOT(tx()));
    connect(&m_socketClient,SIGNAL(rx()),&m_txWidget,SLOT(rx()));
//Botones para la barra de herramientas:
    QToolButton* btnConnect     = new QToolButton(this);
    btnConnect->setIcon(QIcon(":/icons/icons/connect-icon.png"));
    connect(btnConnect,SIGNAL(toggled(bool)),ui->connectionWidget,SLOT(connectSocket(bool)));
    btnConnect->setCheckable(true);

    QToolButton* btnShowSensors = new QToolButton(this);
    btnShowSensors->setText("Sensors");
    btnShowSensors->setCheckable(true);
    btnShowSensors->setChecked(false);
    connect(btnShowSensors,SIGNAL(toggled(bool)),ui->sensorViewWidget,SLOT(setVisible(bool)));
    connect(btnShowSensors,SIGNAL(toggled(bool)),&m_socketClient,SLOT(requestSensorData(bool)));

    QToolButton* btnShowBright  = new QToolButton(this);
    btnShowBright->setText("Bright");
    btnShowBright->setCheckable(true);
    btnShowBright->setChecked(true);
    connect(btnShowBright,SIGNAL(toggled(bool)),ui->groupBrightness,SLOT(setVisible(bool)));

    QToolButton* btnOffsetTool = new QToolButton(this);
    btnOffsetTool->setText("OffsetTool");
    btnOffsetTool->setCheckable(true);
    btnOffsetTool->setChecked(true);
    connect(btnOffsetTool,SIGNAL(toggled(bool)),ui->groupOffset ,SLOT(setVisible(bool)));

    QToolButton* btnShowEffects = new QToolButton(this);
    btnShowEffects->setText("Effects");
    btnShowEffects->setCheckable(true);
    btnShowEffects->setChecked(true);
    connect(btnShowEffects,SIGNAL(toggled(bool)),ui->effectsWidget ,SLOT(setVisible(bool)));

    ui->mainToolBar->addWidget(btnConnect);
    ui->mainToolBar->addWidget(btnShowSensors);
    ui->mainToolBar->addWidget(btnShowBright);
    ui->mainToolBar->addWidget(btnShowEffects);
    ui->mainToolBar->addWidget(btnOffsetTool);
//
    ui->sensorViewWidget->hide();
//    ui->effectsWidget->hide();
//    ui->groupBrightness->hide();
//    ui->groupOffset->hide();

    connect(ui->connectionWidget,SIGNAL(ready())    ,this,SLOT(enableUI()));
    connect(ui->connectionWidget,SIGNAL(notReady()) ,this,SLOT(disableUI()));

    ui->connectionWidget->setClient(&m_socketClient);
    ui->sensorViewWidget->setClient(&m_socketClient);

    connect(ui->btnOff,           SIGNAL(clicked(bool)),&m_socketClient,    SLOT(off()));
    connect(ui->btnNext,          SIGNAL(clicked(bool)),&m_socketClient,    SLOT(nextEffect()));
    connect(ui->btnPrev,          SIGNAL(clicked(bool)),&m_socketClient,    SLOT(prevEffect()));
    connect(ui->btnCircle,        SIGNAL(clicked(bool)),&m_socketClient,    SLOT(animationCircle()));
    connect(ui->btnDoubleCircle,  SIGNAL(clicked(bool)),&m_socketClient,    SLOT(animationDoubleCircle()));
    connect(ui->btnRainbow,       SIGNAL(clicked(bool)),&m_socketClient,    SLOT(animationRainbow()));
    connect(ui->btnFlash ,        SIGNAL(clicked(bool)),&m_socketClient,    SLOT(animationFlash()));
    connect(ui->btnSetColor,      SIGNAL(clicked(bool)),this,               SLOT(setColor()));
    connect(ui->sliderBrightness, SIGNAL(valueChanged(int)),this,           SLOT(setBrightness(int)));
    connect(ui->dialOffset,       SIGNAL(valueChanged(int)),&m_socketClient,SLOT(setOffset(int)));

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
    ui->groupOffset->setEnabled(state);
}

void controlMainWindow::disableUI()
{
    bool state = false;
    ui->sensorViewWidget->setEnabled(state);
    ui->groupBrightness->setEnabled(state);
    ui->effectsWidget->setEnabled(state);
    ui->groupOffset->setEnabled(state);
}
