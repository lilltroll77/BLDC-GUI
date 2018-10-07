#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("FOC & SVPWM for BLDC with XMOS");
    QMenuBar* menuBar = new QMenuBar(this);
    QMenu FileMenu;

    FileMenu.addSection("File");
    FileMenu.addSection("Help");
    FileMenu.set

    menuBar->addMenu(&FileMenu);

    setMenuBar(menuBar);
    currentGauge = new currentGague(this);
    top_layout.addWidget(currentGauge,2,2,1,1);
    top_layout.setAlignment(currentGauge , Qt::AlignBottom);
    top_layout.setSpacing(5);
    //this->setLayout(&top_layout);
    //setCentralWidget(currentGauge);
    this->setLayout(&top_layout);
    //this->update();

}

MainWindow::~MainWindow()
{
    delete ui;
}
