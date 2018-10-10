#ifndef SERIAL_COM_H
#define SERIAL_COM_H
#include <QSerialPort>
#include "currentgague.h"

#define SERIAL_HEADER_SIZE 4
enum COMMAND{LINK_DOWN,
             LINK_UP ,
             COM_CURRENT ,
             COM_STOP,
             COM_DRV,
             COM_DRV_ERROR ,
             COM_DRV_RESET ,
             COM_NEWTEMP ,
             COM_FUSE_BLOWN ,
             COM_FUSE_RESET ,
             COM_FUSE_CURRENT ,
             COM_VDS ,
             COM_ODT ,
             COM_TDRIVE ,
             COM_IDRIVE_P_HS ,
             COM_IDRIVE_N_HS ,
             COM_IDRIVE_P_LS ,
             COM_IDRIVE_N_LS ,
             COM_SET_TORQUE ,
             COM_SET_FLUX ,
             COM_PI_FUSE ,
             COM_PI_TORQUE_FREQ ,
             COM_PI_TORQUE_GAIN ,
             COM_PI_FLUX_FREQ ,
             COM_PI_FLUX_GAIN
            };
void writeSerialCommand(char ID , QSerialPort* serial);
void writeSerialUint(char ID , unsigned value , QSerialPort* serial);
void writeSerialFloat(char ID , float value , QSerialPort* serial);

#endif // SERIAL_COM_H
