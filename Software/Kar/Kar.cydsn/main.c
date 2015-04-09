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

#define HIGH 0
#define LOW 1

typedef struct {
    uint8 addr;
    uint8 status;
    uint8 type;
    uint8 values[2];
} Fieldsensor;

typedef struct {
    uint8 addr;
    Fieldsensor* fieldsensors[10];
    uint8 fieldsensor_connected;
    uint8 status;
    uint8 ventil;
} Sensoroe;

Sensoroe* oer[2];
uint8 oer_connected = 0;
uint8 state = 0;
uint8 polling_nr = 0;

Sensoroe* findSensorOe(uint8 addr)
{
    uint8 i;
    for (i = 0; i < oer_connected; ++i) {
        if (oer[i]->addr == addr)
            return oer[i];
    }
    return NULL;
}

Fieldsensor* findFieldsensor(uint8 oe_addr, uint8 sensor_addr)
{
    uint8 i;
    Sensoroe* oe;
    oe = findSensorOe(oe_addr);
    if (oe != NULL) {
        for (i = 0; i < oe->fieldsensor_connected; ++i) {
            if (oe->fieldsensors[i]->addr == sensor_addr)
                return oe->fieldsensors[i];
        }
    }
    return NULL;    
}
void addSensoroe(uint8 addr) {
    int i;
    oer[oer_connected] = malloc(sizeof(Sensoroe));
    oer[oer_connected]->addr = addr;
    oer[oer_connected]->status = 0;
    oer[oer_connected]->ventil = 0;
    for (i = 0; i < 10; ++i)
        oer[oer_connected]->fieldsensors[i] = 0;
    ++oer_connected;
}

void pollSensoroe()
{
        Debug_PutString("Polling\r\n");
        OEBUS_PutTxMessage(oer[polling_nr]->addr, 0, OEBUS_OE_REQ_FS_DATA);
        state = 1;
        Debug_PutString("State 1");
        if (oer[++polling_nr] == 0)
            polling_nr = 0;
}

void parseOEBUS()
{
    OEBUS_MSG_STRUCT msg;
    int i, j;
    OEBUS_GetRxMessage(&msg);
    #if OEBUS_DEBUG_UART
    OEBUS_DebugMsg(&msg);
    #endif
    
    switch(msg.cmd) {
        case OEBUS_OE_RES_FS_DATA:
            Debug_PutString("Got response\r\n");
            for ( i = 0; i < oer_connected; ++i) {
                if (oer[i]->addr == msg.transmitter) {
                    //Not pretty as we overwrite and dont update
                    oer[i]->fieldsensor_connected = msg.len/4;
                    for (j=0; j<oer[i]->fieldsensor_connected; ++j) {
                        if (oer[i]->fieldsensors[j] == NULL)
                            oer[i]->fieldsensors[j] = malloc(sizeof(Fieldsensor));
                        
                        oer[i]->fieldsensors[j]->addr           = msg.args[j * 4 + 0];
                        oer[i]->fieldsensors[j]->status         = msg.args[j * 4 + 1];
                        oer[i]->fieldsensors[j]->values[HIGH]   = msg.args[j * 4 + 2];
                        oer[i]->fieldsensors[j]->values[LOW]    = msg.args[j * 4 + 3];
                    }
                    break;
                }
            }
            state = 0;
            Debug_PutString("State 0");
            break;
        case OEBUS_OE_RES_VENTIL:
            Debug_PutString("Got response\r\n");
            for ( i = 0; i < oer_connected; ++i) {
                if (oer[i]->addr == msg.transmitter) {
                    oer[i]->ventil = msg.args[0];
                    break;
                }
            }
            state = 0;
            Debug_PutString("State 0");
            break;
        default:
            break;
    }
    OEBUS_ClearRxMessage();
}

void parseKARBUS()
{
    KARBUS_MSG_STRUCT msg;
    int i;
    Sensoroe* oe;
    Fieldsensor* fs;
    
    KARBUS_GetRxMessage(&msg);
    #if KARBUS_DEBUG_UART
    KARBUS_DebugMsg(&msg);
    #endif
    
    switch(msg.cmd) {
        case KARBUS_REQ_KAR_VENTIL:
            if (msg.args[0] == 1)
                Indloeb_ChangeState(msg.args[1]);
            else
                Afloeb_ChangeState(msg.args[1]);
            KARBUS_PutTxMessage(msg.transmitter,1,KARBUS_RES_KAR_VENTIL);
            KARBUS_PutTxMessageArg(msg.args[1]);
            break;
        case KARBUS_REQ_KAR_OE_LIST:
            KARBUS_PutTxMessage(msg.transmitter,oer_connected,KARBUS_RES_KAR_OE_LIST);
            for (i = 0; i < oer_connected; ++i)
                KARBUS_PutTxMessageArg(oer[i]->addr);
            break;
        case KARBUS_REQ_KAR_OE_SENSOR_TYPE:
            fs = findFieldsensor(msg.args[0], msg.args[1]);
            if (fs != NULL) {
                KARBUS_PutTxMessage(msg.transmitter,1,KARBUS_RES_KAR_OE_SENSOR_TYPE);
                KARBUS_PutTxMessageArg(fs->type);
            } else {
                KARBUS_PutTxMessage(msg.transmitter,0,KARBUS_REQ_KAR_NONE);
            }
            break;
        case KARBUS_REQ_KAR_OE_VENTIL:
            oe = findSensorOe(msg.args[0]);
            if (oe != NULL) {
                OEBUS_PutTxMessage(msg.args[0], 1, OEBUS_OE_REQ_VENTIL);
                OEBUS_PutTxMessageArg(msg.args[1]);
                KARBUS_PutTxMessage(msg.transmitter, 1, KARBUS_RES_KAR_OE_VENTIL);
                KARBUS_PutTxMessageArg(msg.args[1]);
            } else {
                KARBUS_PutTxMessage(msg.transmitter,0,KARBUS_REQ_KAR_NONE);
            }
        case KARBUS_REQ_KAR_OE_SENSOR_DATA:
            oe = findSensorOe(msg.args[0]);
            if (oe != NULL) {
                KARBUS_PutTxMessage(msg.transmitter,oe->fieldsensor_connected*4,KARBUS_RES_KAR_OE_SENSOR_DATA);
                for (i = 0; i < oe->fieldsensor_connected; ++i) {
                    KARBUS_PutTxMessageArg(oe->fieldsensors[i]->addr);
                    KARBUS_PutTxMessageArg(oe->fieldsensors[i]->status);
                    KARBUS_PutTxMessageArg(oe->fieldsensors[i]->values[HIGH]);
                    KARBUS_PutTxMessageArg(oe->fieldsensors[i]->values[LOW]);
                }
            } else {
                KARBUS_PutTxMessage(msg.transmitter,0,KARBUS_REQ_KAR_NONE);
            }
            break;
        case KARBUS_REQ_KAR_AKTUATOR_DATA:
            KARBUS_PutTxMessage(msg.transmitter,1,KARBUS_RES_KAR_AKTUATOR_DATA);
            KARBUS_PutTxMessageArg(Pumpe_GetSpeed());
            break;
        case KARBUS_REQ_KAR_SENSOR_DATA:
            KARBUS_PutTxMessage(msg.transmitter,0,KARBUS_REQ_KAR_NONE);
            break;
        default:
            break;
    }
    KARBUS_ClearRxMessage();
}

#if KARBUS_DEBUG_UART || OEBUS_DEBUG_UART
void debugUart() {
    char ch;
    char debug_msg[50];
    ch = Debug_GetChar();
    
    switch(ch)
    {
        case 'h':
            Debug_PutString("Commands\r\n\r\n");
            Debug_PutString("0-5\tShow SensorOe 0-9\r\n");
            
            break;
        case 'o':
            
            break;
        case 'O':
            
            break;
        case 'f':
            
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
            
            break;
        default:
            break;
    }
    
}
#endif

int main(void)
{
    #if KARBUS_DEBUG_UART || OEBUS_DEBUG_UART
    Debug_Start();
    #endif
    
    #if KARBUS_DEBUG_UART
    KARBUS_DebugInit(Debug_PutString);
    #endif
    
    #if OEBUS_DEBUG_UART
    OEBUS_DebugInit(Debug_PutString);
    #endif
    
    addSensoroe(0x04);
    
    OEBUS_Start();
    
    KARBUS_Start();
    
    CyGlobalIntEnable;
    
    for(;;)
    {
        
        #if KARBUS_DEBUG_UART || OEBUS_DEBUG_UART
        debugUart();
        #endif

       if (OEBUS_ReadRxStatus() == OEBUS_MSG_READY)
            parseOEBUS();
        
        if (KARBUS_ReadRxStatus() == KARBUS_MSG_READY)
            parseKARBUS();
        
        if (state == 0 && oer[polling_nr] != 0)
            pollSensoroe();
        
        CyDelay(1);
    }
}

/* [] END OF FILE */
