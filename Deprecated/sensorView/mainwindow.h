#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qserialport.h>
#include <qmutex.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void parseRxBuffer();
    Ui::MainWindow *ui;
    QSerialPort m_serial;
    QMutex      m_mutex;
    QString     m_buffer;
    int axmin=0,axmax=0,aymin=0,aymax=0,azmin=0,azmax=0,gxmin=0,gxmax=0,gymin=0,gymax=0,gzmin=0,gzmax=0,vmin=0,vmax=0;
signals:

public slots:
    void readSerial();
    void on_connectBtn_clicked();

};

#endif // MAINWINDOW_H
