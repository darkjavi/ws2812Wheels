#include "controlmainwindow.h"
#include "ui_controlmainwindow.h"

controlMainWindow::controlMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::controlMainWindow)
{
    ui->setupUi(this);
}

controlMainWindow::~controlMainWindow()
{
    delete ui;
}
