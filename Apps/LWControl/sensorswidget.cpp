#include "sensorswidget.h"
#include "ui_sensorswidget.h"

sensorsWidget::sensorsWidget(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::sensorsWidget)
{
    ui->setupUi(this);
    addGraphs();
}

sensorsWidget::~sensorsWidget()
{
    delete ui;
}

void sensorsWidget::sensorRead(int Gx, int Gy, int Gz, int AcX, int AcY, int AcZ, float Temp, int vuLevel)
{
    ui->dial_gx->setValue(Gx);
    ui->valueLabel_gx->setText(QString::number(Gx));
    m_graph_gx->appendData(QVector<double>() << Gx);

    ui->dial_gy->setValue(Gy);
    ui->valueLabel_gy->setText(QString::number(Gy));
    m_graph_gy->appendData(QVector<double>() << Gy);

    ui->dial_gz->setValue(Gz);
    ui->valueLabel_gz->setText(QString::number(Gz));
    m_graph_gz->appendData(QVector<double>() << Gz);

    ui->progressBar_acx->setValue(AcX);
    ui->valueLabel_acx->setText(QString::number(AcX));

    ui->progressBar_acy->setValue(AcY);
    ui->valueLabel_acy->setText(QString::number(AcY));

    ui->progressBar_acz->setValue(AcZ);
    ui->valueLabel_acz->setText(QString::number(AcZ));

    ui->progressBar_temp->setValue(Temp);
    ui->valueLabel_temp->setText(QString::number(Temp));

    ui->progressBar_vuLevel->setValue(vuLevel);
    ui->valueLabel_vuLevel->setText(QString::number(vuLevel));
}


void sensorsWidget::addGraphs()
{
    m_graph_gx = new gSimpleGraph(ui->graficos,QVector<QString>()<<QString("Gx"),500);
    m_graph_gx->appendData(QVector<double>()<< 0);
    m_graph_gx->setMinimumHeight(60);
    ui->graficos->layout()->addWidget(m_graph_gx);

    m_graph_gy = new gSimpleGraph(ui->graficos,QVector<QString>()<<QString("Gy"),500);
    m_graph_gy->appendData(QVector<double>()<< 0);
    m_graph_gy->setMinimumHeight(60);
    ui->graficos->layout()->addWidget(m_graph_gy);

    m_graph_gz = new gSimpleGraph(ui->graficos,QVector<QString>()<<QString("Gz"),500);
    m_graph_gz->appendData(QVector<double>()<< 0);
    m_graph_gz->setMinimumHeight(60);
    ui->graficos->layout()->addWidget(m_graph_gz);
}
