#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serial = new QSerialPort(this);
    setWindowTitle("FOC & SVPWM for BLDC with XMOS");
    top_layout = new QGridLayout(this);
    top_box = new QGroupBox(this);

    currentGauge = new currentGague(this);
    tempGauge = new tempgauge(this);
    angleGauge = new QEgauge(this);
    tachometerGauge = new tachometer(this);

    statusbar = new QStatusBar(this);
    menuBar = new menu(this);
    updateStatusBar(disconnected);
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

    // ******** For testing purpose ***************

    struct I_t I ={1.0f , 0.0f};
    float T=0 , deg=0 , RPM=0;


/*
    while(1){
        wait(16);
        tempGauge->setTemp(T+20.0f);
        angleGauge->setAngle(deg);
        currentGauge->setCurrent(&I);
        tachometerGauge->setSpeed(RPM);

        I.peak = fmodf(I.peak+0.1f , 25.0f);
        I.ms = fmodf(I.ms+0.1f , 25.0f);
        T = fmodf(T+0.25f , 100.0f);
        RPM = fmodf(RPM+5 , 3000.0f);
        deg = fmodf(deg+ 1.0f , 360.0f);
        //readCOMdata();

    }
 */
}


MainWindow::~MainWindow()
{
    writeSerialCommand(CLOSE_LINK , serial);
    serial->waitForBytesWritten();
    serial->close();
    delete ui;

}

void MainWindow::openCOMport(QSerialPortInfo portInfo){
    if(!serial->isOpen()){
        serial->setPort(portInfo);
        QErrorMessage msg;
        msg.setWindowTitle("COM port error");
        if(!serial->open(QSerialPort::ReadWrite)){
            msg.showMessage("COM port could not open");
            msg.exec();
        }
        else{
            while(!serial->isOpen())
                wait(1);
            //serial->blockSignals(true);
            serial->setReadBufferSize(512);
            serial->clear(QSerialPort::AllDirections);
            serial->clearError();
            //serial->setBreakEnabled(false);
            connect(serial , &QSerialPort::errorOccurred , this , &MainWindow::COMError);
            connect(serial , &QSerialPort::readyRead , this , &MainWindow::readCOMdata );
            //serial->blockSignals(false);
            writeSerialUint(LINK_VER, magicID , serial);
            qDebug() << "Link ver. command sent";
            //readCOMdata();
        }

    }
    else{
        QErrorMessage msg;
        msg.showMessage("COM port already open");
        msg.exec();
    }

}

void MainWindow::readCOMdata(){
    if(!serial->isOpen())
         return;
    enum BYTES{LENGTH , COMMAND , DATATYPE , CHECKSUM};
    char serial_data[512];
    I_t* I;

    qint64 bytes = serial->bytesAvailable();
    if(bytes > 0){
        qDebug() <<"Bytes:" <<bytes;
    }
    while( bytes >= SERIAL_HEADER_SIZE){
        if(serial->read(serial_data , SERIAL_HEADER_SIZE)!=SERIAL_HEADER_SIZE)
            qDebug() <<"Header read error";
        int len = serial_data[LENGTH] - SERIAL_HEADER_SIZE;
        int command = serial_data[COMMAND];
        qDebug()<< "Recieved: Package len"<<len << "COM:"<<command;
        float* float_vec;
        unsigned* u32_data;
        if(len>0){
            while( serial->bytesAvailable() < len)
                wait(1);
            if(serial->read(serial_data , len) != len)
                qDebug() << "Header read error";
            float_vec = reinterpret_cast<float*>(serial_data);
            I =         reinterpret_cast<struct I_t*>(serial_data);
            u32_data = reinterpret_cast<unsigned*>(serial_data);
        }

        /// ADD SCHECKSUM!

        switch(command){
        case(CLOSE_LINK):{
            serial->close();
            updateStatusBar(disconnected);
            break;
        }
        case(OPEN_LINK):{
            updateStatusBar(connected);
            break;
        }
        case(LINK_VER):{
            if(link_version == *u32_data){
                qDebug() << "Open link command sent";
                writeSerialCommand(OPEN_LINK , serial);
                link_up=true;
                updateStatusBar(connected);
            }else{
                QString str;
                if(link_version > *u32_data)
                    str = "GUI code version is newer than XMOS code version. Update XMOS code!";
                else
                    str = "GUI code version is older than XMOS code version. Update GUI code!";
                QErrorMessage msg;
                msg.showMessage(str);
                msg.setWindowTitle("Error");
                msg.exec();
            }
            break;
        }
        case(COM_CURRENT):{
            currentGauge->setCurrent(I);
            break;
        }
        default:
            qDebug()<<"Unknown COM command";
            break;
        }//switch
    bytes = serial->bytesAvailable();
    }// while
}

void MainWindow::COMError(QSerialPort::SerialPortError error){
    QString str;
    switch(error){
    case QSerialPort::NoError:
        return;
    case QSerialPort::DeviceNotFoundError:
        str="An error occurred while attempting to open an non-existing device.";
        break;
    case QSerialPort::PermissionError:
        str="An error occurred while attempting to open an already opened device by another process or a user not having enough permission and credentials to open.";
        break;
    case QSerialPort::OpenError:
        str="An error occurred while attempting to open an already opened device in this object.";
        break;
    case QSerialPort::WriteError:
        str = "Write error";
        break;
    case QSerialPort::ReadError:
         str = "Read error";
        break;
    case QSerialPort::ResourceError:
         str = "An I/O error occurred when a resource becomes unavailable, e.g. when the device is unexpectedly removed from the system.";
        break;
    case QSerialPort::TimeoutError:
         str = "A timeout error occurred";
    default:
        str.sprintf("Error %d: Read the Qt manual :) @ enum QSerialPort::SerialPortError" , error);
        break;
    }
    serial->clearError();
    QErrorMessage msg;
    msg.showMessage(str);
    msg.setWindowTitle("COM port error");
    msg.exec();


}

void MainWindow::updateStatusBar(enum status_event event){
    switch(event){
    case connected:{
        statusbar->showMessage("CONNECTED TO XMOS", 0);
        statusbar->setStyleSheet("color:green");
        break;
    }
    case disconnected:{
        statusbar->showMessage("NOT CONNECTED" , 0);
        statusbar->setStyleSheet("color:red");
        statusbar->show();
        break;
    }
    }
    statusbar->show();
}
