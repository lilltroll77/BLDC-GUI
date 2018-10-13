#include "serial_com.h"
#include <QTime>
#include <QCoreApplication>
#include <QDebug>
#include <QSerialPortInfo>
#include <QtWidgets>


extern void wait( int millisecondsToWait );


enum DATA_TYPE{FIXED_POINT , FLOATING_POINT};
enum BYTES{LENGTH , COMMAND , DATATYPE , CHECKSUM};

void insertChecksum(char* data , int len){
    int checksum=0;
    data[CHECKSUM] =0;
    for(int i=0; i<len; i++)
        checksum += reinterpret_cast<unsigned char*>(data)[i];
    reinterpret_cast<unsigned char*>(data)[CHECKSUM]= checksum & 0xFF;
}

void writeSerialCommand(char command , QSerialPort* serial){
    const int len = SERIAL_HEADER_SIZE;
    char data[len];
    data[LENGTH] = len;
    data[COMMAND] = command;
    data[DATATYPE] = FIXED_POINT;
    insertChecksum(data , len);
    if(serial->write(data , len) !=len)
        qDebug()<< "Error in writeSerialCommand";
}

void writeSerialUint(char command  , unsigned value , QSerialPort* serial){
    const int len = SERIAL_HEADER_SIZE + sizeof(float);
    char data[len]={len , 0 , FIXED_POINT};
    data[COMMAND] = command ;
    reinterpret_cast<unsigned*>(data)[1] = value;
    insertChecksum(data , len);
    if(serial->write(data , len) !=len)
        qDebug()<< "Error in writeSerialUint";
}

void writeSerialFloat(char command , float value , QSerialPort* serial){
    const qint64 len = SERIAL_HEADER_SIZE + sizeof(float);
    char data[len]={len};
    data[COMMAND] = command;
    data[DATATYPE] = FLOATING_POINT;
    reinterpret_cast<float*>(data)[1] = value;
    insertChecksum(data , len);
    if(serial->write(data , len) !=len)
        qDebug()<< "Error in writeSerialFloat";
}



