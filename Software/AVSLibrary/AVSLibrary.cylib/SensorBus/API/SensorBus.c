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

//Debug variables
#if `$INSTANCE_NAME`_DEBUG_UART
#include "..\..\Includes\avs_debug.h"
#endif

//Genreal Variables
uint8  `$INSTANCE_NAME`_address;

volatile uint8  `$INSTANCE_NAME`_value_high;
volatile uint8  `$INSTANCE_NAME`_value_low;

//I2C Variabler
volatile uint8 `$INSTANCE_NAME`_state = `$INSTANCE_NAME`_REQ_VALUE;
volatile uint8 `$INSTANCE_NAME`_rdBuffer[3] = {0};
volatile uint8 `$INSTANCE_NAME`_wrBuffer[3] = {0};

void `$INSTANCE_NAME`_Start()
{
    #if `$INSTANCE_NAME`_DEBUG_UART
    #if `$INSTANCE_NAME`_FIELDSENSOR
        printf("\n\r\n\rStarting Fieldsensor\n\r\n\r");
    #else
        printf("\n\r\n\rStarting Sensor OE\n\r\n\r");
    #endif
    #endif
    
    `$INSTANCE_NAME`_I2C_I2CSlaveInitReadBuf((uint8 *) `$INSTANCE_NAME`_rdBuffer, 3);
    `$INSTANCE_NAME`_I2C_I2CSlaveInitWriteBuf((uint8 *) `$INSTANCE_NAME`_wrBuffer, 3);
    `$INSTANCE_NAME`_I2C_Start();
    `$INSTANCE_NAME`_ChangeAddress(`$INSTANCE_NAME`_ADDRESS_INIT);
    
    #if `$INSTANCE_NAME`_FIELDSENSOR
    `$INSTANCE_NAME`_LoadValue(32,128);
    #endif
    
    #if `$INSTANCE_NAME`_DEBUG_UART
    #if `$INSTANCE_NAME`_FIELDSENSOR
        printf("\n\rFieldsensor [ 0x%02X ] - Ready\n\r\n\r", `$INSTANCE_NAME`_GetAddress());
    #else
        printf("\n\rSensoroe [ 0x%02X ] - Ready\n\r\n\r", `$INSTANCE_NAME`_GetAddress());
    #endif
    #endif
}

void `$INSTANCE_NAME`_Communicate()
{
    `$INSTANCE_NAME`_ParseRead();
    `$INSTANCE_NAME`_ParseWrite();
}

uint8 `$INSTANCE_NAME`_Read(uint8 addr, uint8 * str, uint8 len, uint8 blocking_mode)
{
    uint32 status;
    uint8 i;
    
    if (blocking_mode) {
        `$INSTANCE_NAME`_I2C_I2CMasterReadBuf(addr, str, len, `$INSTANCE_NAME`_I2C_I2C_MODE_COMPLETE_XFER);
        while (!(`$INSTANCE_NAME`_I2C_I2CMasterStatus() & `$INSTANCE_NAME`_I2C_I2C_MSTAT_RD_CMPLT));
        return 1;
    } else {
        status = `$INSTANCE_NAME`_I2C_I2CMasterSendStart(addr, `$INSTANCE_NAME`_I2C_I2C_READ_XFER_MODE);
        if(status == `$INSTANCE_NAME`_I2C_I2C_MSTR_NO_ERROR) {
            for(i = 0; i < len - 1; ++i) {
                str[i] = `$INSTANCE_NAME`_I2C_I2CMasterReadByte(`$INSTANCE_NAME`_I2C_I2C_ACK_DATA);
            }
            str[i] = `$INSTANCE_NAME`_I2C_I2CMasterReadByte(`$INSTANCE_NAME`_I2C_I2C_NAK_DATA);
        }
        
        `$INSTANCE_NAME`_I2C_I2CMasterSendStop();
        `$INSTANCE_NAME`_I2C_EnableInt();
        return (status == `$INSTANCE_NAME`_I2C_I2C_MSTR_NO_ERROR);
    }
}

uint8 `$INSTANCE_NAME`_Write(uint8 addr, uint8 * str, uint8 len, uint8 blocking_mode)
{
    uint32 status;
    uint8 i;
    
    if (blocking_mode) {
        `$INSTANCE_NAME`_I2C_I2CMasterWriteBuf(addr, str, len, `$INSTANCE_NAME`_I2C_I2C_MODE_COMPLETE_XFER);
        while (!(`$INSTANCE_NAME`_I2C_I2CMasterStatus() & `$INSTANCE_NAME`_I2C_I2C_MSTAT_WR_CMPLT));
        return 1;
    } else {
        status = `$INSTANCE_NAME`_I2C_I2CMasterSendStart(addr, `$INSTANCE_NAME`_I2C_I2C_WRITE_XFER_MODE);
        if(status == `$INSTANCE_NAME`_I2C_I2C_MSTR_NO_ERROR) {
            for(i = 0; i < len; ++i) {
                `$INSTANCE_NAME`_I2C_I2CMasterWriteByte(str[i]);
            }
        }
        `$INSTANCE_NAME`_I2C_I2CMasterSendStop();
        `$INSTANCE_NAME`_I2C_EnableInt();
        return (status == `$INSTANCE_NAME`_I2C_I2C_MSTR_NO_ERROR);
    }
}

uint8 `$INSTANCE_NAME`_ParseRead()
{
    if(0u != (`$INSTANCE_NAME`_I2C_I2CSlaveClearReadStatus() & `$INSTANCE_NAME`_I2C_I2C_SSTAT_RD_CMPLT))
    {
        `$INSTANCE_NAME`_I2C_I2CSlaveClearReadBuf();
        return 1;
    }
    return 0;
}

#if `$INSTANCE_NAME`_FIELDSENSOR
uint8 `$INSTANCE_NAME`_ParseWrite()
{
    uint8 reply;
    uint8 cmd;
    
    //Hvis der er kommet et write til os
    if(0u != (`$INSTANCE_NAME`_I2C_I2CSlaveClearWriteStatus() & `$INSTANCE_NAME`_I2C_I2C_SSTAT_WR_CMPLT))
    {
        cmd = *`$INSTANCE_NAME`_wrBuffer;
        
        switch(cmd) 
        {
            case `$INSTANCE_NAME`_REQ_VALUE:
            case `$INSTANCE_NAME`_REQ_TYPE:
                
                //Skift state baseret på commandoen vi fik
                `$INSTANCE_NAME`_ChangeMode(cmd);
                
                //Clear bufferen, da alle I2C vi har er på én byte
                `$INSTANCE_NAME`_I2C_I2CSlaveClearWriteBuf();
                
                //Reply master with notits
                reply = `$INSTANCE_NAME`_RES_READY;
                `$INSTANCE_NAME`_Write(`$INSTANCE_NAME`_MASTER_ID, &reply, 1, 1);
                
                #if `$INSTANCE_NAME`_DEBUG_UART
                printf("Replied master\t[ 0x01 ]\r\n");
                #endif
            default:
                break;
        }
        return 1;
    }
    return 0;
}

#else
    
void  `$INSTANCE_NAME`_Scan()
{
    //0x09 - 0x78
    uint8 scan_addr;
    uint8 dummy;
    printf("Scanning\n\r");
    for (scan_addr = 0x08; scan_addr <= 0x78; ++scan_addr) {
        if (scan_addr == `$INSTANCE_NAME`_GetAddress())
            continue;
        
        if (`$INSTANCE_NAME`_Read(scan_addr, &dummy, 1, 0)) {
            printf("Found Sensor:\t [ 0x%02X ]\n\r", scan_addr);
        }
    }
    printf("Done\n\r");
}
    
uint8 `$INSTANCE_NAME`_ParseWrite()
{
    uint8 res = 0;
    if(0u != (`$INSTANCE_NAME`_I2C_I2CSlaveClearWriteStatus() & `$INSTANCE_NAME`_I2C_I2C_SSTAT_WR_CMPLT))
    {
        
        res = *`$INSTANCE_NAME`_wrBuffer;
        `$INSTANCE_NAME`_I2C_I2CSlaveClearWriteBuf();
        return res == `$INSTANCE_NAME`_RES_READY;
    }
    return 0;
}
#endif

uint8 `$INSTANCE_NAME`_GetAddress()
{
    return `$INSTANCE_NAME`_address;
}

void `$INSTANCE_NAME`_ChangeAddress(uint8 addr)
{
    `$INSTANCE_NAME`_address = addr;
    `$INSTANCE_NAME`_I2C_I2CSlaveSetAddress(`$INSTANCE_NAME`_address);
    #if `$INSTANCE_NAME`_DEBUG_UART
    printf("Address changed\t[ 0x%02X ]\r\n", `$INSTANCE_NAME`_address);
    #endif
}

#if `$INSTANCE_NAME`_FIELDSENSOR
    
void `$INSTANCE_NAME`_ChangeMode(uint8 mode)
{
    switch(mode)
    {
        case `$INSTANCE_NAME`_REQ_TYPE:
           `$INSTANCE_NAME`_state = `$INSTANCE_NAME`_REQ_TYPE;
            break;
        case `$INSTANCE_NAME`_REQ_VALUE:
            `$INSTANCE_NAME`_state = `$INSTANCE_NAME`_REQ_VALUE;
            break;
        default:
            break;
    }
    
    #if `$INSTANCE_NAME`_DEBUG_UART
    printf("\r\nChange mode\t[ 0x%02X ]\r\n", `$INSTANCE_NAME`_state);
    #endif
    
    //Refresh the buffer if we changed
    `$INSTANCE_NAME`_RefreshData();
    
    
}  

void `$INSTANCE_NAME`_RefreshData()
{
    switch(`$INSTANCE_NAME`_state)
    {
        case `$INSTANCE_NAME`_REQ_VALUE:
            `$INSTANCE_NAME`_rdBuffer[0] = `$INSTANCE_NAME`_value_high;
            `$INSTANCE_NAME`_rdBuffer[1] = `$INSTANCE_NAME`_value_low;
            break;
        case `$INSTANCE_NAME`_REQ_TYPE:
            `$INSTANCE_NAME`_rdBuffer[0] = `$INSTANCE_NAME`_TYPE;
            break;
        default:
            break;
            
    }
}

void `$INSTANCE_NAME`_LoadValue(uint8 high, uint8 low)
{
    `$INSTANCE_NAME`_value_high = high;
    `$INSTANCE_NAME`_value_low = low;
    `$INSTANCE_NAME`_RefreshData();
}

void `$INSTANCE_NAME`_LoadValue_Float32(float32 value)
{
    `$INSTANCE_NAME`_LoadValue(value, ((uint8)(value * 2) & 1 ) << 7);
}

uint8 `$INSTANCE_NAME`_GetHigh(uint8 buffer)
{
    if (buffer)
        return `$INSTANCE_NAME`_rdBuffer[0];
    else 
        return `$INSTANCE_NAME`_value_high;
}

uint8 `$INSTANCE_NAME`_GetLow(uint8 buffer)
{
    if (buffer)
        return `$INSTANCE_NAME`_rdBuffer[1];
    else
        return `$INSTANCE_NAME`_value_low;
}

#else
    
void  `$INSTANCE_NAME`_SensorChangeMode(uint8 addr, uint8 mode)
{
    `$INSTANCE_NAME`_ParseWrite();
    `$INSTANCE_NAME`_Write(addr, &mode, 1, 0);
    while (!`$INSTANCE_NAME`_ParseWrite());
}
    
uint8  `$INSTANCE_NAME`_SensorPoll(uint8 addr, uint8 *values, uint8 *type)
{    
    if(`$INSTANCE_NAME`_Read(addr, values, 2, 0)) {
        if (*type == `$INSTANCE_NAME`__UNKNOWN) {
            `$INSTANCE_NAME`_SensorChangeMode(addr, `$INSTANCE_NAME`_REQ_TYPE);
            `$INSTANCE_NAME`_Read(addr, type, 1, 0);
            `$INSTANCE_NAME`_SensorChangeMode(addr, `$INSTANCE_NAME`_REQ_VALUE);
        }
        return 1;
    }
    return 0;
}
    
#endif

#if `$INSTANCE_NAME`_DEBUG_UART

void `$INSTANCE_NAME`_DebugRead(uint8 len)
{
    uint8 data[len];
    uint8 i;
    uint8 addr;
    
    #if `$INSTANCE_NAME`_FIELDSENSOR
    addr = 0x08;
    #else
    addr = 0x09;
    #endif
    
    if (`$INSTANCE_NAME`_Read(addr, data, len, 0)) {
        printf("Read\t");
        for (i = 0; i < len; ++i) {
            printf("[ 0x%02X ] ", data[i]);
        }
        printf("\n\r\n\r");
    } else {
        printf("Error reading\n\r\n\r");
    }
}

void  `$INSTANCE_NAME`_DebugHandle(const char ch)
{
    uint8 dummy_data[3] = {0};

    if(ch != 0u) {
        switch(ch)
        {
            case 27:
                printf("`$INSTANCE_NAME`");
                break;
            case '?':
                printf("\n\r\n\rHelp:\n\r\n\r");
                printf("T\tShow Type\n\r");
                printf("A\tShow Address\n\r");
                printf("b\tShow wrBuffer\n\r");
                printf("B\tShow rdBuffer\n\r");
                printf("1-3\tI2C Read\n\r");
                printf("4\tI2C Write\n\r");
                
                #if `$INSTANCE_NAME`_FIELDSENSOR
                    printf("V\tShow Values\n\r");
                    printf("v\tChange mode [VALUE]\n\r");
                    printf("t\tChange mode [TYPE]\n\r");
                #else
                    printf("s\tScan subnet\n\r");
                    printf("v\tI2C REQ_VALUE\n\r");
                    printf("t\tI2C REQ_TYPE\n\r");
                    printf("p\tI2C POLL\n\r");    
                #endif
                
                printf("\n\r");
                break;
            case 'A':
                printf("Address is\t[ 0x%02X ]\n\r", `$INSTANCE_NAME`_GetAddress());
                break;
            case 'T':
                printf("Type contains\t[ 0x%02X ]\n\r", `$INSTANCE_NAME`_TYPE );
                break;
            case 'b':
                printf("wrBuffer contains\t[ 0x%02X ] [ 0x%02X ] [ 0x%02X ]\n\r", `$INSTANCE_NAME`_wrBuffer[0], `$INSTANCE_NAME`_wrBuffer[1], `$INSTANCE_NAME`_wrBuffer[2] );
                break;
            case 'B':
                printf("rdBuffer contains\t[ 0x%02X ] [ 0x%02X ] [ 0x%02X ]\n\r", `$INSTANCE_NAME`_rdBuffer[0], `$INSTANCE_NAME`_rdBuffer[1], `$INSTANCE_NAME`_rdBuffer[2] );
                break;
            case '1':
            case '2':
            case '3':
                `$INSTANCE_NAME`_DebugRead(ch - '0');
                break;
            case '4':
                dummy_data[0] = `$INSTANCE_NAME`_RES_READY;
                `$INSTANCE_NAME`_Write(0x08, dummy_data, 1, 1);
                break;
            #if `$INSTANCE_NAME`_FIELDSENSOR
            case 'V':
                printf("Values contains\t[ 0x%02X ] [ 0x%02X ]\n\r",`$INSTANCE_NAME`_GetHigh(0), `$INSTANCE_NAME`_GetLow(0));
               break;
            case 'v':
                `$INSTANCE_NAME`_ChangeMode(`$INSTANCE_NAME`_REQ_VALUE);
               break;
            case 't':
                `$INSTANCE_NAME`_ChangeMode(`$INSTANCE_NAME`_REQ_TYPE);
                 break;
            #else
            case 's':
                `$INSTANCE_NAME`_Scan();
                break;
            case 'p':
                dummy_data[2] = `$INSTANCE_NAME`__UNKNOWN;
                printf("Polling\t[ 0x09 ]\n\r");
                `$INSTANCE_NAME`_SensorPoll(0x09, dummy_data, &dummy_data[2]);
                printf("Read\t[ 0x%02X ] [ 0x%02X ]\n\r", dummy_data[0], dummy_data[1]);
                printf("Read\t[ 0x%02X ]\n\r\n\r", dummy_data[2]);
               break;
            case 'v':
                `$INSTANCE_NAME`_SensorChangeMode(0x09, `$INSTANCE_NAME`_REQ_VALUE);
               break;
            case 't':
                `$INSTANCE_NAME`_SensorChangeMode(0x09, `$INSTANCE_NAME`_REQ_TYPE);
                 break;    
            #endif
        }    
    }
}
#endif
/* [] END OF FILE */
