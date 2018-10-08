#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "currentgague.h"
#include "tempgauge.h"
#include "QEgauge.h"

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

private:
    Ui::MainWindow *ui;
    QGridLayout* top_layout;
    QGroupBox* top_box;
    currentGague* currentGauge;
    tempgauge* tempGauge;
    QEgauge* angleGauge;

};

#endif // MAINWINDOW_H
