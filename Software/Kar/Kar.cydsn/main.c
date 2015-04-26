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

#include "..\..\Includes\avs_enums.h"
#include "..\..\Includes\avs_structs.h"
#include "..\..\Includes\avs_debug.h"
#include "..\..\Includes\avs_debug.c"

Sensoroe* oer[10];
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

void addSensoroe(uint8 addr)
{
    int i;
    oer[oer_connected] = malloc(sizeof(Sensoroe));
    oer[oer_connected]->addr = addr;
    oer[oer_connected]->status = 0;
    oer[oer_connected]->ventil = 0;
    oer[oer_connected]->fieldsensor_connected = 0;
    for (i = 0; i < 10; ++i)
        oer[oer_connected]->fieldsensors[i] = 0;
    ++oer_connected;
}

void pollSensoroe()
{
        OEBUS_PutTxMessage(oer[polling_nr]->addr, 0, REQ_OE_FS_DATA);
        state = 1;
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
        case REQ_OE_FS_DATA:
            //Debug_PutString("Got response\r\n");
            for ( i = 0; i < oer_connected; ++i) {
                if (oer[i]->addr == msg.transmitter) {
                    //Not pretty as we overwrite and dont update
                    oer[i]->fieldsensor_connected = msg.len/4;
                    for (j=0; j<oer[i]->fieldsensor_connected; ++j) {
                        if (oer[i]->fieldsensors[j] == NULL)
                            oer[i]->fieldsensors[j] = malloc(sizeof(Fieldsensor));
                        
                        oer[i]->fieldsensors[j]->addr           = msg.args[j * 4 + 0];
                        oer[i]->fieldsensors[j]->status         = msg.args[j * 4 + 1];
                        oer[i]->fieldsensors[j]->values[FS_VALUE_HIGH]   = msg.args[j * 4 + 2];
                        oer[i]->fieldsensors[j]->values[FS_VALUE_LOW]    = msg.args[j * 4 + 3];
                    }
                    break;
                }
            }
            state = 0;
            //Debug_PutString("State 0");
            break;
        case REQ_OE_VENTIL:
            printf("Got response\r\n");
            for ( i = 0; i < oer_connected; ++i) {
                if (oer[i]->addr == msg.transmitter) {
                    oer[i]->ventil = msg.args[0];
                    break;
                }
            }
            state = 0;
            printf("State 0");
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
    CyDelay(500);
    switch(msg.cmd) {
        case REQ_KAR_VENTIL:
            if (msg.args[0] == 1)
                Indloeb_SetState(msg.args[1]);
            else
                Afloeb_SetState(msg.args[1]);
            KARBUS_PutTxMessage(msg.transmitter,1,RES_KAR_VENTIL);
            KARBUS_PutTxMessageArg(msg.args[1]);
            break;
        case REQ_KAR_OE_LIST:
            KARBUS_PutTxMessage(msg.transmitter,oer_connected, RES_KAR_OE_LIST);
            for (i = 0; i < oer_connected; ++i)
                KARBUS_PutTxMessageArg(oer[i]->addr);
            break;
        case REQ_KAR_OE_SENSOR_TYPE:
            fs = findFieldsensor(msg.args[0], msg.args[1]);
            if (fs != NULL) {
                KARBUS_PutTxMessage(msg.transmitter,1,RES_KAR_OE_SENSOR_TYPE);
                KARBUS_PutTxMessageArg(fs->type);
            } else {
                KARBUS_PutTxMessage(msg.transmitter,0,REQ_KAR_NONE);
            }
            break;
        case REQ_KAR_OE_VENTIL:
            oe = findSensorOe(msg.args[0]);
            if (oe != NULL) {
                OEBUS_PutTxMessage(msg.args[0], 1, REQ_OE_VENTIL);
                OEBUS_PutTxMessageArg(msg.args[1]);
                KARBUS_PutTxMessage(msg.transmitter, 1, RES_KAR_OE_VENTIL);
                KARBUS_PutTxMessageArg(msg.args[1]);
            } else {
                KARBUS_PutTxMessage(msg.transmitter,0,REQ_KAR_NONE);
            }
        case REQ_KAR_OE_SENSOR_DATA:
            oe = findSensorOe(msg.args[0]);
            if (oe != NULL) {
                KARBUS_PutTxMessage(msg.transmitter,oe->fieldsensor_connected*4,RES_KAR_OE_SENSOR_DATA);
                for (i = 0; i < oe->fieldsensor_connected; ++i) {
                    KARBUS_PutTxMessageArg(oe->fieldsensors[i]->addr);
                    KARBUS_PutTxMessageArg(oe->fieldsensors[i]->status);
                    KARBUS_PutTxMessageArg(oe->fieldsensors[i]->values[FS_VALUE_HIGH]);
                    KARBUS_PutTxMessageArg(oe->fieldsensors[i]->values[FS_VALUE_LOW]);
                }
            } else {
                KARBUS_PutTxMessage(msg.transmitter,0,REQ_KAR_NONE);
            }
            break;
        case REQ_KAR_PUMPE:
            if (msg.args[0] == 0) {
                Pumpe_Block();
            } else {
                Pumpe_SetSpeed(msg.args[0] * 30);
                Pumpe_Start();
            }
            KARBUS_PutTxMessage(msg.transmitter,1,RES_KAR_PUMPE);
            KARBUS_PutTxMessageArg(msg.args[0]);
            break;
        case REQ_KAR_SENSOR_DATA:
            KARBUS_PutTxMessage(msg.transmitter,0,REQ_KAR_NONE);
            break;
        case REQ_KAR_OPRET:
            addSensoroe(msg.args[0]);
            KARBUS_PutTxMessage(msg.transmitter,0,RES_KAR_OPRET);
            break;
        default:
            break;
    }
    KARBUS_ClearRxMessage();
}

#if defined(DEBUG_UART)

void debugListFieldsensors(Sensoroe* oe)
{
    uint8 i;
    for (i=0; i < oe->fieldsensor_connected; ++i)
    {
        printf("Addr:\t [ 0x%02X ]\t Status: [ 0x%02X ]\n\r", oe->fieldsensors[i]->addr, oe->fieldsensors[i]->status);
    }
}

void debugSensorOe(Sensoroe* oe)
{
    printf("SensorOe config\n\r\n\r");
    printf("Kar Addr:\t [ 0x%02X ]\n\r", oe->addr);
    printf("Ventil State:\t [ 0x%02X ]\n\r", oe->ventil);
    printf("Fieldsensors:\t [ 0x%02X ]\n\r", oe->fieldsensor_connected);
    printf("====================================================\n\r");
    debugListFieldsensors(oe);
}

void debugListSensorOes()
{
    uint8 i;
    for (i=0; i < oer_connected; ++i)
    {
        printf("%d\t Addr:\t [ 0x%02X ]\t Status: [ 0x%02X ]\n\r", i, oer[i]->addr, oer[i]->status);
    }
}

void debugConfig()
{
    printf("Kar config\n\r\n\r");
    printf("Kar Addr:\t [ 0x%02X ]\n\r", KARBUS_GetAddress());
    Indloeb_DebugState();
    Afloeb_DebugState();
    printf("SensorOes:\t [ 0x%02X ]\n\r", oer_connected);
    printf("====================================================\n\r");
    debugListSensorOes();
}    
    
void DebugHandle(const char ch) {
    Sensoroe* oe = NULL;
    
    switch(ch)
    {
        case 27:
            printf("Kar");
            break;
        case '?':
            printf("Commands\n\r\n\r");
            printf("0-9\tShow SensorOe 0-9\n\r");
            printf("a\tAdd SensorOe 0x04\n\r");
            printf("l\tList SensorOes\n\r");
            printf("p\tPrint config\n\r");
            printf("\n\r");
            break;
        case 'a':
            addSensoroe(0x04);
            break;
        case 'l':
            debugListSensorOes();
            break;
        case 'p':
            debugConfig();
            break;
        case '0'...'9':
            oe = oer[ch - '0'];
            if (oe != NULL)
                debugSensorOe(oe);
            break;
        default:
            printf("Not implemented\n\r");
    }
}


#endif

int main(void)
{
    #if defined(DEBUG_UART)
        Clock_Start();
        Debug_Start();
        Debug_AddComponent(DebugHandle);
        #if Pumpe_DEBUG_UART
            Debug_AddComponent(Pumpe_DebugHandle);
        #endif
        #if Indloeb_DEBUG_UART
            Debug_AddComponent(Indloeb_DebugHandle);
        #endif
        #if Afloeb_DEBUG_UART
            Debug_AddComponent(Afloeb_DebugHandle);
        #endif
        #if OEBUS_DEBUG_UART
            Debug_AddComponent(OEBUS_DebugHandle);
        #endif
        #if KARBUS_DEBUG_UART
            Debug_AddComponent(KARBUS_DebugHandle);
        #endif
    #endif
    
    OEBUS_Start();
    KARBUS_Start();
    Pumpe_Start();
    Indloeb_Start();
    Afloeb_Start();
    
    CyGlobalIntEnable;
    
    for(;;)
    {
        #if defined(DEBUG_UART)
        Debug_Communicate();
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
