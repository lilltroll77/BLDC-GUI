#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "currentgague.h"

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

};

#endif // MAINWINDOW_H
