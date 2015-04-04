/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>
#include <stdlib.h>

#include "../../Includes/Fieldsensor.h"
#include "../../Includes/Sensor_Bus.h"
#include "../../Includes/OE_Bus.h"

#ifdef DEBUG
#include <stdio.h>
#endif

enum {
    I2C_OFFLINE = 0,
    I2C_ONLINE = 1,
};

typedef struct {
    uint8 id;
    uint8 status;
    uint8 type;
    uint8 value_high;
    uint8 value_low;
} Fieldsensor;

volatile Fieldsensor* fieldsensors[10];
volatile uint8 fieldsensor_connected = 0;
volatile RS485Bus* oebus;
volatile uint8 ventil = 0;

void sendFieldsensors(volatile RS485Bus* q, uint8 receiver);

#ifdef DEBUG
char debug_msg[50];
void printMessage(volatile RS485Bus* bus);
#endif

CY_ISR_PROTO(RX485);

//Debug UART
#ifdef DEBUG    
void printRS485Bus(volatile RS485Bus* bus) {
        uint16 len;
        uint16 i = 0;
        len = bus->msg[RS_LEN_HIGH];
        len <<= 8;
        len += bus->msg[RS_LEN_LOW];
        sprintf(debug_msg, "Status:\t\t0x%02X (", bus->status);
        UART_PC_PutString(debug_msg);
        UART_PC_PutString(msg_status_str(bus->status));
        UART_PC_PutString(")\r\n");
        sprintf(debug_msg, "Receiver:\t0x%02X\r\n", bus->msg[RS_RECEIVER]);
        UART_PC_PutString(debug_msg);
        sprintf(debug_msg, "Sender:\t\t0x%02X\r\n", bus->msg[RS_TRANSMITTER]);
        UART_PC_PutString(debug_msg);
        sprintf(debug_msg, "Len High:\t%d\r\n", bus->msg[RS_LEN_HIGH]);
        UART_PC_PutString(debug_msg);
        sprintf(debug_msg, "Len Low:\t%d\r\n", bus->msg[RS_LEN_LOW]);
        UART_PC_PutString(debug_msg);
        sprintf(debug_msg, "Len Toal:\t%d\r\n", len);
        UART_PC_PutString(debug_msg);
        sprintf(debug_msg, "Missing Args:\t%d\r\n", bus->missing_args);
        UART_PC_PutString(debug_msg);
        sprintf(debug_msg, "Command:\t0x%02X (", bus->msg[RS_CMD]);
        UART_PC_PutString(debug_msg);
        UART_PC_PutString(oeb_cmd_str(bus->msg[RS_CMD]));
        UART_PC_PutString(")\r\n");
        UART_PC_PutString("Message:\t");
        for(i = 0; i < len; ++i) {
            sprintf(debug_msg, "0x%02X ", bus->msg[RS_ARG_START+i]);
            UART_PC_PutString(debug_msg); 
        }
        UART_PC_PutString("\r\n\r\n");
}
    
void debugUart() {
    char ch;
    ch = UART_PC_GetChar();
    volatile Fieldsensor* fs = 0;
    switch(ch)
    {
        case '?':
            UART_PC_PutString("Commands\r\n\r\n");
            UART_PC_PutString("0-5\tShow Fieldsensor 0-9\r\n");
            UART_PC_PutString("k\tShow karbus message\r\n");
            UART_PC_PutString("c\tClear karbus message\r\n");
            UART_PC_PutString("t\tTransmit all Fieldsensors to 0x04\r\n");
            UART_PC_PutString("r\tRequest Fieldsensors from 0x03\r\n");
            UART_PC_PutString("o\tOpen ventil at 0x03\r\n");
            UART_PC_PutString("O\tClose ventil at 0x03\r\n");
            UART_PC_PutString(".\tToggle address 0x03 0x04\r\n");
            UART_PC_PutString("\r\n");
            break;
        case 'o':
            UART_PC_PutString("Opening ventil\r\n\r\n");
            sendCommand(oebus, 0x03, 1, OEB_REQ_VENTIL);
            sendArg(oebus,1);
            break;
        case 'O':
            UART_PC_PutString("Closing ventil\r\n\r\n");
            sendCommand(oebus, 0x03, 1, OEB_REQ_VENTIL);
            sendArg(oebus,0);
            break;
        case 'r':
            UART_PC_PutString("Requesting sensors\r\n\r\n");
            sendCommand(oebus, 0x03, 0, OEB_REQ_FS_DATA);
            break;
        case '.':
            if (oebus->busid == 0x03)
                oebus->busid = 0x04;
            else
                oebus->busid = 0x03;
            OEBUS_Slave_UartSetRxAddress(oebus->busid);
            sprintf(debug_msg, "ID: 0x%02X\r\n\r\n", oebus->busid);
            UART_PC_PutString(debug_msg);
            
            break;
        case 'k':
            printRS485Bus(oebus);
            break;
        case 'c':
            clearRS485Bus(oebus);
            printRS485Bus(oebus);
            break;
        case 't':
            sendFieldsensors(oebus, 0x04);
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
            fs = fieldsensors[ch - '0'];
            if (fs != 0) {
                UART_PC_PutString("Sensor\t\t0\r\n");
                  
                sprintf(debug_msg, "ID\t\t0x%02X\r\n", fs->id);
                UART_PC_PutString(debug_msg);
                     
                sprintf(debug_msg, "Status:\t\t0x%02X (", fs->status);
                UART_PC_PutString(debug_msg);
                switch(fs->status)
                {
                    case I2C_OFFLINE:
                        UART_PC_PutString("OFFLINE");
                        break;
                    case I2C_ONLINE:
                        UART_PC_PutString("ONLINE");
                        break;
                    default:
                        UART_PC_PutString("UNKNOWN");
                        break;
                }
                UART_PC_PutString(")\r\n");
                              
                sprintf(debug_msg, "Type:\t\t0x%02X (", fs->type);
                UART_PC_PutString(debug_msg);
                UART_PC_PutString(fs_types_str(fs->type));
                UART_PC_PutString(")\r\n");
                       
                sprintf(debug_msg, "Value High:\t%d\r\n", fs->value_high);
                UART_PC_PutString(debug_msg);
                sprintf(debug_msg, "Value Low:\t%d\r\n", fs->value_low);
                UART_PC_PutString(debug_msg);
                sprintf(debug_msg, "Value Total:\t%d,%d\r\n\r\n", fs->value_high, (fs->value_low>>7)*5);
                UART_PC_PutString(debug_msg);
            } else {
                UART_PC_PutString("Sensor does not exists\r\n\r\n");
            }
            break;
        default:
            break;
    }
    
}

#endif

void changeVentil(uint8 state) {
    ventil = state;    
}

void addFieldsensor(uint8 id) {
    fieldsensors[fieldsensor_connected] = malloc(sizeof(Fieldsensor));
    fieldsensors[fieldsensor_connected]->id = id;
    fieldsensors[fieldsensor_connected]->status = I2C_OFFLINE;
    fieldsensors[fieldsensor_connected]->type = unknown;
    fieldsensors[fieldsensor_connected]->value_high = 0;
    fieldsensors[fieldsensor_connected]->value_low = 0;
    ++fieldsensor_connected;
}

void pollFieldsensor(volatile Fieldsensor* fs) {
    uint32 status;
    uint8 cmd;
    
    status = SENSORBUS_Master_I2CMasterSendStart(fs->id, SENSORBUS_Master_I2C_READ_XFER_MODE);
    if(status == SENSORBUS_Master_I2C_MSTR_NO_ERROR) {
        fs->value_high = SENSORBUS_Master_I2CMasterReadByte(SENSORBUS_Master_I2C_ACK_DATA);
        fs->value_low = SENSORBUS_Master_I2CMasterReadByte(SENSORBUS_Master_I2C_NAK_DATA);
    }
    SENSORBUS_Master_I2CMasterSendStop();
    
    if(status == SENSORBUS_Master_I2C_MSTR_NO_ERROR)
    {
        if (fs->type == unknown || fs->status == I2C_OFFLINE) {
            
            //Change slave to send type
            cmd = returnTypes;
            SENSORBUS_Master_I2CMasterWriteBuf(0x08, &cmd, 1, SENSORBUS_Master_I2C_MODE_COMPLETE_XFER);
            while (!(SENSORBUS_Master_I2CMasterStatus() & SENSORBUS_Master_I2C_MSTAT_WR_CMPLT));
            
            //Dummy read - Timing issue?
            SENSORBUS_Master_I2CMasterReadBuf(0x08, &cmd, 1, SENSORBUS_Master_I2C_MODE_COMPLETE_XFER);
            while (!(SENSORBUS_Master_I2CMasterStatus() & SENSORBUS_Master_I2C_MSTAT_RD_CMPLT));
            
            //Correct read
            SENSORBUS_Master_I2CMasterReadBuf(0x08, &cmd, 1, SENSORBUS_Master_I2C_MODE_COMPLETE_XFER);
            while (!(SENSORBUS_Master_I2CMasterStatus() & SENSORBUS_Master_I2C_MSTAT_RD_CMPLT));
            fs->type = cmd;
            
            //Change slave back to sending values
            cmd = returnValues;
            SENSORBUS_Master_I2CMasterWriteBuf(0x08, &cmd, 1, SENSORBUS_Master_I2C_MODE_COMPLETE_XFER);
            while (!(SENSORBUS_Master_I2CMasterStatus() & SENSORBUS_Master_I2C_MSTAT_WR_CMPLT));
        }
        fs->status = I2C_ONLINE;
    } else {
        fs->status = I2C_OFFLINE;
    }
}

void sendFieldsensors(volatile RS485Bus* q, uint8 receiver) {
    uint8 i = 0;
    while(fieldsensors[i] != 0) {++i;}
    sendCommand(q, receiver, i*4, OEB_RES_FS_DATA);
    i = 0;
    while(fieldsensors[i] != 0) {
        sendArg(q, fieldsensors[i]->id);
        sendArg(q, fieldsensors[i]->status);
        sendArg(q, fieldsensors[i]->value_high);
        sendArg(q, fieldsensors[i]->value_low);
        ++i;
    }
}

void parseRS485Bus(volatile RS485Bus* bus) {
    if (bus->status == RS_MSG_READY) {
        switch(bus->msg[RS_CMD]) {
            case OEB_REQ_FS_DATA:
                sendFieldsensors(bus, bus->msg[RS_TRANSMITTER]);
                break;
            case OEB_REQ_VENTIL:
                changeVentil(bus->msg[RS_ARG_START]);
                sendCommand(bus, bus->msg[RS_TRANSMITTER], 1, OEB_RES_VENTIL);
                sendArg(bus, ventil);
                break;
            default:
                break;
        }
        #ifdef DEBUG
        printRS485Bus(bus);
        #endif
        clearRS485Bus(bus);
    }
}

CY_ISR(RX485) {
    pushRS485Bus(oebus, OEBUS_Slave_UartGetChar());
    OEBUS_Slave_ClearRxInterruptSource(OEBUS_Slave_INTR_RX_NOT_EMPTY);
}

int main()
{
    
    uint8 polling_nr = 0;
    
    //Populate variables
    initRS485Bus(&oebus, 0x03, OEBUS_Slave_SpiUartWriteTxData);
    addFieldsensor(0x08);
    addFieldsensor(0x09);
    addFieldsensor(0x0A);
    addFieldsensor(0x0B);
    
    //Initialise components
    SENSORBUS_Master_Start();
    OEBUS_Slave_Start();
    isr_RS485_StartEx(RX485);
    
    #ifdef DEBUG
    Clock_Start();
    UART_PC_Start();
    UART_PC_PutString("Initializing: Done\r\n");
    #endif
    
    OEBUS_Slave_UartSetRxAddress(oebus->busid);
    
    CyGlobalIntEnable;
    
    for(;;)
    {
        #ifdef DEBUG
        debugUart();
        #endif

        verifyRS485Bus(oebus);
        
        parseRS485Bus(oebus);
        
        //We poll a single sensor, before we loop around, to be sure we don't spend to much time polling
        //Could be empty array if no sensors are added
        if (fieldsensors[polling_nr] != 0)
            pollFieldsensor(fieldsensors[polling_nr]);
        if ( fieldsensors[++polling_nr] == 0 )
            polling_nr = 0;
        
        CyDelay(50);
    }
}

/* [] END OF FILE */
