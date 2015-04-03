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

#define FS_STRINGS
#include "../../AVS_Header.h"

#ifdef DEBUG
#include <stdio.h>
#endif

enum {
    RX_IDLE = 0,
    RX_LEN_HIGH = 1,
    RX_LEN_LOW = 2,
    RX_CMD = 3,
    RX_ARGS = 4,
    RX_READY = 5,
};

enum {
    I2C_OFFLINE = 0,
    I2C_ONLINE = 1,
};

typedef struct
{
    uint8 id;
    uint8 status;
    uint8 type;
    uint8 value_high;
    uint8 value_low;
} Fieldsensor;

volatile Fieldsensor* fieldsensors[10];

//Debug UART
#ifdef DEBUG
void debugUart() {
    char ch;
    char debug_msg[50];
    ch = UART_PC_GetChar();
    switch(ch)
    {
        case '?':
            UART_PC_PutString("Commands\r\n\r\n");
            UART_PC_PutString("1 Change state to returnValues\r\n");
            UART_PC_PutString("2 Change state to returnTypes\r\n");
            UART_PC_PutString("s Show State\r\n");
            UART_PC_PutString("v Show Values\r\n");
            UART_PC_PutString("t Show Type\r\n");
            UART_PC_PutString("l Load dummy values\r\n");
            UART_PC_PutString("c Clear values\r\n");
            UART_PC_PutString("\r\n");
            break;
        default:
            break;
    }
}
#endif

void addFieldsensor(uint8 id) {
    uint8 i = 0;
    while(fieldsensors[i] != 0) {++i;}
    fieldsensors[i] = malloc(sizeof(Fieldsensor));
    fieldsensors[i]->id = id;
}

void pollFieldsensor(volatile Fieldsensor* fs) {
    uint32 status;
    uint8 cmd;
    
    status = OEBUS_Master_I2CMasterSendStart(fs->id, OEBUS_Master_I2C_READ_XFER_MODE);
    if(status == OEBUS_Master_I2C_MSTR_NO_ERROR) {
        fs->value_high = OEBUS_Master_I2CMasterReadByte(OEBUS_Master_I2C_ACK_DATA);
        fs->value_low = OEBUS_Master_I2CMasterReadByte(OEBUS_Master_I2C_NAK_DATA);
    }
    OEBUS_Master_I2CMasterSendStop();
    
    if(status == OEBUS_Master_I2C_MSTR_NO_ERROR)
    {
        if (fs->type == unknown || fs->status == I2C_OFFLINE) {
            
            //Change slave to send type
            cmd = returnTypes;
            OEBUS_Master_I2CMasterWriteBuf(0x08, &cmd, 1, OEBUS_Master_I2C_MODE_COMPLETE_XFER);
            while (!(OEBUS_Master_I2CMasterStatus() & OEBUS_Master_I2C_MSTAT_WR_CMPLT));
            
            //Dummy read - Timing issue?
            OEBUS_Master_I2CMasterReadBuf(0x08, &cmd, 1, OEBUS_Master_I2C_MODE_COMPLETE_XFER);
            while (!(OEBUS_Master_I2CMasterStatus() & OEBUS_Master_I2C_MSTAT_RD_CMPLT));
            
            //Correct read
            OEBUS_Master_I2CMasterReadBuf(0x08, &cmd, 1, OEBUS_Master_I2C_MODE_COMPLETE_XFER);
            while (!(OEBUS_Master_I2CMasterStatus() & OEBUS_Master_I2C_MSTAT_RD_CMPLT));
            fs->type = cmd;
            
            //Change slave back to sending values
            cmd = returnValues;
            OEBUS_Master_I2CMasterWriteBuf(0x08, &cmd, 1, OEBUS_Master_I2C_MODE_COMPLETE_XFER);
            while (!(OEBUS_Master_I2CMasterStatus() & OEBUS_Master_I2C_MSTAT_WR_CMPLT));
        }
        fs->status = I2C_ONLINE;
    } else {
        fs->status = I2C_OFFLINE;
    }
}

int main()
{
    uint8 polling_nr = 0;
    
    #ifdef DEBUG
    Clock_Start();
    UART_PC_Start();
    UART_PC_PutString("Initializing: Done\r\n");
    #endif
    
    addFieldsensor(0x08);
    
    OEBUS_Master_Start();
    
    CyGlobalIntEnable;
    
    
    for(;;)
    {
        debugUart();
        //Could be empty
        if (fieldsensors[polling_nr] != 0)
            pollFieldsensor(fieldsensors[polling_nr]);
    
        CyDelay(50);
        
    
        if ( fieldsensors[++polling_nr] == 0 )
            polling_nr = 0;
    }
}

/* [] END OF FILE */
