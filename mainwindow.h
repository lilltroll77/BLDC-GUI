#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "currentgague.h"
#include "tempgauge.h"
#include "QEgauge.h"
#include "tachometer.h"
#include "menu.h"
#include "serial_com.h"


extern void wait( int millisecondsToWait );

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    currentGague* currentGauge;

private slots:
    void openCOMport(QSerialPortInfo portInfo);
    void readCOMdata();
    void COMError(QSerialPort::SerialPortError error);

private:
    Ui::MainWindow *ui;
    QGridLayout* top_layout;
    QGroupBox* top_box;
    tempgauge* tempGauge;
    QEgauge* angleGauge;
    tachometer* tachometerGauge;
    QStatusBar* statusbar;
    menu* menuBar;
    QSerialPort serial;

};

#endif // MAINWINDOW_H
