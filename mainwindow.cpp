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
    top_layout->addWidget(currentGauge->widget);
    top_box->setLayout(top_layout);
    this->setCentralWidget(top_box );
    this->show();

    struct I_t I ={1.0f , 0.0f};
    while(1){
        wait(16);
        I.peak = fmodf(I.peak+0.1f , 25.0f);
        I.ms = fmodf(I.ms+0.1f , 25.0f);
        currentGauge->setCurrent(&I);


    }

}

MainWindow::~MainWindow()
{
    delete ui;
}
