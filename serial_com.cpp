#include "serial_com.h"
#include <QTime>
#include <QCoreApplication>
#include <QDebug>
#include <QSerialPortInfo>
#include <QtWidgets>


extern void wait( int millisecondsToWait );


enum DATA_TYPE{FIXED_POINT , FLOATING_POINT};

void writeSerialCommand(char ID , QSerialPort* serial){
    const int len = 4;
    char data[len]={len};
    data[1] = ID;
    serial->write(data , len);
}

void writeSerialUint(char ID , unsigned value , QSerialPort* serial){
    const int len = SERIAL_HEADER_SIZE + sizeof(float);
    char data[len]={len};
    data[1] = ID;
    data[2] = FIXED_POINT;
    reinterpret_cast<unsigned*>(data)[0] = value;
    serial->write(data , len);
}

void writeSerialFloat(char ID , float value , QSerialPort* serial){
    const int len = SERIAL_HEADER_SIZE + sizeof(float);
    char data[len]={len};
    data[1] = ID;
    data[2] = FLOATING_POINT;
    reinterpret_cast<float*>(data)[0] = value;
    serial->write(data , len);
}



