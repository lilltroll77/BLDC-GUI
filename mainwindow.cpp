#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("FOC & SVPWM for BLDC with XMOS");
    top_layout = new QGridLayout(this);
    top_box = new QGroupBox(this);

    currentGauge = new currentGague(this);
    tempGauge = new tempgauge(this);
    angleGauge = new QEgauge(this);
    statusbar = new QStatusBar(this);
    statusbar->showMessage("Serial: Not connected");
    top_layout->addWidget(currentGauge->widget , 1,1);
    top_layout->addWidget(tempGauge->widget , 2,1);
    top_layout->addWidget(angleGauge->widget , 2 ,2);
    top_box->setLayout(top_layout);
    top_box->setTitle("Gauges");
    this->setCentralWidget(top_box );
    this->setStatusBar(statusbar);
    this->show();

    // ******** For testing purpose ***************

    struct I_t I ={1.0f , 0.0f};
    float T=0 , deg=0;

    QTime dieTime = QTime::currentTime().currentTime();
    while(1){
        dieTime = dieTime.addMSecs(16);
        while( QTime::currentTime() < dieTime )
            QCoreApplication::processEvents( QEventLoop::AllEvents, 4);


        I.peak = fmodf(I.peak+0.1f , 25.0f);
        I.ms = fmodf(I.ms+0.1f , 25.0f);
        currentGauge->setCurrent(&I);
        T = fmodf(T+0.25f , 100.0f);
        tempGauge->setTemp(T+20.0f);
        deg = fmodf(deg+ 1.0f , 360.0f);
        angleGauge->setAngle(deg);


    }

}

MainWindow::~MainWindow()
{
    delete ui;
}
