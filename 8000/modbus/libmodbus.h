#ifndef LIBMODBUS_H
#define LIBMODBUS_H
#include <QtCore/qglobal.h>
#include<QObject>
typedef unsigned char uint8_t;
#define MAX_LEN  1024

enum ModbusConnectionType {
    COMRTU,
    Tcp,
    COMASCII
};

struct ModbusComPortInfo
{
    int port;
    int baudrate;
    int databits;
    int stopbits;
    int parity;
};

struct ModbusTcpInfo
{
    QString address;
    int portNbr;
};

struct ModbusDataInfo
{
    int slaveID;
    int functionCode;
    int startAddr;
    int nums;
    int data[MAX_LEN];
};

class LibModbus;
//extern LibModbus *g_LibModbus;

class LibModbus: public QObject
{
    Q_OBJECT

public:
    LibModbus();
    ~LibModbus();
    void SetModbusConnectionType(ModbusConnectionType type);
    void CopyDataToSendBuff(ModbusDataInfo data);
    bool TcpModbusCreate(ModbusTcpInfo tcpinfo);
    bool SerialModbusCreate(ModbusComPortInfo cominfo);
    void releaseModbus();
    void sendModbusRequest( void );
    void emit_recevieModbusData(uint8_t * data, int dataLen,bool addNewline);
signals:
    void sig_ModbusData(uint8_t * data, int dataLen,bool addNewline);
private slots:
    void pollForDataOnBus( void );
};
#endif
