#ifndef CONTROLMAINWINDOW_H
#define CONTROLMAINWINDOW_H

#include <QMainWindow>

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
    Ui::controlMainWindow *ui;
};

#endif // CONTROLMAINWINDOW_H
