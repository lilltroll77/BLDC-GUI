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
    tachometerGauge = new tachometer(this);

    statusbar = new QStatusBar(this);
    menuBar = new menu(this);
    statusbar->showMessage("Serial: Not connected");
    top_layout->addWidget(currentGauge->widget , 1,1);
    top_layout->addWidget(tempGauge->widget , 2,1);
    top_layout->addWidget(tachometerGauge->widget , 1, 2);
    top_layout->addWidget(angleGauge->widget , 2 ,2);
    top_box->setLayout(top_layout);
    top_box->setTitle("Gauges");
    this->setCentralWidget(top_box );
    this->setStatusBar(statusbar);
    this->setMenuBar(menuBar->bar);
    this->show();

    connect(menuBar , SIGNAL(COMportSelected(QSerialPortInfo)) , this , SLOT(openCOMport(QSerialPortInfo)));
 /// ERROR in signal   connect(&serial  , SIGNAL(errorOccurred(SerialPortError)) , this , SLOT(COMError(QSerialPort::SerialPortError)));
    // ******** For testing purpose ***************

    struct I_t I ={1.0f , 0.0f};
    float T=0 , deg=0 , RPM=0;

    QTime dieTime = QTime::currentTime().currentTime();
    while(1){
        dieTime = dieTime.addMSecs(16);
        while( QTime::currentTime() < dieTime )
            QCoreApplication::processEvents( QEventLoop::AllEvents, 4);
        tempGauge->setTemp(T+20.0f);
        angleGauge->setAngle(deg);
        currentGauge->setCurrent(&I);
        tachometerGauge->setSpeed(RPM);

        I.peak = fmodf(I.peak+0.1f , 25.0f);
        I.ms = fmodf(I.ms+0.1f , 25.0f);
        T = fmodf(T+0.25f , 100.0f);
        RPM = fmodf(RPM+5 , 3000.0f);
        deg = fmodf(deg+ 1.0f , 360.0f);
        readCOMdata();

    }

}

MainWindow::~MainWindow()
{
    serial.close();
    delete ui;

}

void MainWindow::openCOMport(QSerialPortInfo portInfo){
    if(!serial.isOpen()){
        serial.setPort(portInfo);
        QErrorMessage msg;
        msg.setWindowTitle("COM port error");
        if(!serial.open(QSerialPort::ReadWrite)){
            msg.showMessage("COM port could not open");
            msg.exec();
        }
        else{
            serial.setReadBufferSize(512);
            serial.flush();
            writeSerialCommand(LINK_UP, &serial);
        }

    }
    else{
        QErrorMessage msg;
        msg.showMessage("COM port already open");
        msg.exec();
    }

}

void MainWindow::readCOMdata(){
    char serial_data[512];
    I_t* I;
    //qDebug() <<serial.bytesAvailable();
    while((serial.bytesAvailable()>SERIAL_HEADER_SIZE) && (serial.isOpen())){
        serial.read(serial_data , SERIAL_HEADER_SIZE);
        int len = serial_data[0] - SERIAL_HEADER_SIZE;
        qDebug() <<len;
        int command = serial_data[1];
        float* float_vec;
        if(len>0){
            while( serial.bytesAvailable() < len)
                wait(1);
            serial.read(serial_data , len);
            float_vec = reinterpret_cast<float*>(serial_data);
            I =         reinterpret_cast<struct I_t*>(serial_data);
        }

        switch(command){
        case(COM_CURRENT):{
            currentGauge->setCurrent(I);
            break;
        }
        default:
            qDebug()<<"Unknown COM command";
            break;
        }//switch
    }
}

void MainWindow::COMError(QSerialPort::SerialPortError error){
    QString str;
    switch(error){
    case QSerialPort::DeviceNotFoundError:
        str.append("An error occurred while attempting to open an non-existing device.");
        break;
    case QSerialPort::PermissionError:
        str.append("An error occurred while attempting to open an already opened device by another process or a user not having enough permission and credentials to open.");
        break;
    case QSerialPort::OpenError:
        str.append("An error occurred while attempting to open an already opened device in this object.");
        break;
    default:
        str.append("Read the Qt manual :) @ enum QSerialPort::SerialPortError");
        break;
    }
    QErrorMessage msg;
    msg.showMessage(str);
    msg.setWindowTitle("COM port error");
    msg.exec();

}
