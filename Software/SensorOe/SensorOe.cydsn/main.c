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
#include <stdio.h>
#include <stdlib.h>

#include "../../command_list.h"

CY_ISR_PROTO(UART_SLAVE_RX);

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

void addFieldsensor(uint8 id) {
    uint8 i = 0;
    while(fieldsensors[i] != 0) {}
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
    
    addFieldsensor(0x08);
    
    Clock_Start();
    OEBUS_Master_Start();
    UART_PC_Start();
  
    CyGlobalIntEnable;
    
    UART_PC_PutString("Initializing: Done\r\n");
    for(;;)
    {
        //Could be empty
        if (fieldsensors[polling_nr] != 0)
            pollFieldsensor(fieldsensors[polling_nr]);
    
        CyDelay(50);
        
    
        if ( fieldsensors[++polling_nr] == 0 )
            polling_nr = 0;
    }
}

/* [] END OF FILE */
