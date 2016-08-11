#ifndef CONTROLMAINWINDOW_H
#define CONTROLMAINWINDOW_H

#include <QMainWindow>
#include "gwidgettxrx.h"
#include "socketclient.h"

namespace Ui {
class controlMainWindow;
}

class controlMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit controlMainWindow(QWidget *parent = 0);
    ~controlMainWindow();

private:
    Ui::controlMainWindow   *ui;
    socketClient            m_socketClient;
    gWidgetTxRx             m_txWidget;

protected slots:
    void setBrightness(int value);
    void setColor();

    void enableUI();
    void disableUI();
};

#endif // CONTROLMAINWINDOW_H
