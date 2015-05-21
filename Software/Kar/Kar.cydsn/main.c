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
uint8 busy = 0;
uint8 polling_nr = 0;
uint16 flowLiter = 0;
uint16 pHValue = 0;
uint8 flowHigh = 0, flowLow = 0, pHHigh = 0, pHLow = 0;
uint8 pollEnabled = 1;

void pollKarSensor()
{
    float temp_ph, temp_float;
    uint8 float_whole;
    flowLiter = FlowSensor_getFlowLiters();
    flowHigh = (flowLiter >> 8);
    flowLow  = flowLiter;
    temp_ph = pHProbe_getpH();
    float_whole = (uint8)temp_ph;
    pHValue = (float_whole << 8); 
    if(float_whole < temp_ph) {
        temp_float = temp_ph - float_whole;
        if(temp_float > 0.5) {
            pHValue = pHValue | (1 << 7);
        }
    }
    pHHigh = (pHValue >> 8);
    pHLow  = pHValue;
}

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
        busy = 1;
        OEBUS_PutTxMessage(oer[polling_nr]->addr, 0, REQ_OE_FS_DATA);
        state = 1;
        if (oer[++polling_nr] == 0)
            polling_nr = 0;
}

void handleReqKarSensorData(KARBUS_MSG_STRUCT* msg)
{
    KARBUS_PutTxMessage(msg->transmitter,6,RES_KAR_SENSOR_DATA);
    KARBUS_PutTxMessageArg(SENSOR_PHPROBE);
    KARBUS_PutTxMessageArg(pHHigh);
    KARBUS_PutTxMessageArg(pHLow);
    KARBUS_PutTxMessageArg(SENSOR_FLOWMAALER);
    KARBUS_PutTxMessageArg(flowHigh);
    KARBUS_PutTxMessageArg(flowLow);
}

void handleReqKarVentil(KARBUS_MSG_STRUCT* msg)
{
    uint8 state;
    switch(msg->args[0])
    {
        case VENTIL_INDLOEB:
            Indloeb_SetState(msg->args[1]);
            state = Indloeb_GetState();
            break;
        case VENTIL_AFLOEB:
            Afloeb_SetState(msg->args[1]);
            state = Afloeb_GetState();
            break;
        default:
            KARBUS_PutTxMessage(msg->transmitter,0,REQ_KAR_NONE);
            return;
            break;
    }   
    KARBUS_PutTxMessage(msg->transmitter,1,RES_KAR_VENTIL);
    KARBUS_PutTxMessageArg(state);
}

void handleReqKarPumpe(KARBUS_MSG_STRUCT* msg)
{
    if (msg->args[0] == 0) {
        printf("Block\n\r");
        Pumpe_Block();
    } else if (msg->args[0] <= 100) {
        printf("Running\n\r");
        Pumpe_SetSpeed(msg->args[0]);
        Pumpe_Run();
    } else {
        printf("Errors\n\r");
        KARBUS_PutTxMessage(msg->transmitter,0,REQ_KAR_NONE);
        return;
    }
    KARBUS_PutTxMessage(msg->transmitter,1,RES_KAR_PUMPE);
    KARBUS_PutTxMessageArg(Pumpe_GetSpeed());
}

void handleReqKarOpret(KARBUS_MSG_STRUCT* msg)
{
    //Missing test of oe
    addSensoroe(msg->args[0]);
    KARBUS_PutTxMessage(msg->transmitter,0,RES_KAR_OPRET);
}

void handleReqKarOeList(KARBUS_MSG_STRUCT* msg)
{
    uint8 i;
    KARBUS_PutTxMessage(msg->transmitter, oer_connected, RES_KAR_OE_LIST);
    for (i = 0; i < oer_connected; ++i)
        KARBUS_PutTxMessageArg(oer[i]->addr);
}

void handleReqKarOeSensorData(KARBUS_MSG_STRUCT* msg)
{
    Sensoroe* oe;
    uint8 i;
    oe = findSensorOe(msg->args[0]);
    if (oe != NULL) {
        KARBUS_PutTxMessage(msg->transmitter, oe->fieldsensor_connected*4, RES_KAR_OE_SENSOR_DATA);
        for (i = 0; i < oe->fieldsensor_connected; ++i) {
            KARBUS_PutTxMessageArg(oe->fieldsensors[i]->type);
            KARBUS_PutTxMessageArg(oe->fieldsensors[i]->status);
            KARBUS_PutTxMessageArg(oe->fieldsensors[i]->values[FS_VALUE_HIGH]);
            KARBUS_PutTxMessageArg(oe->fieldsensors[i]->values[FS_VALUE_LOW]);            
        }
    } else {
        KARBUS_PutTxMessage(msg->transmitter,0,REQ_KAR_NONE);
    }
}

void handleReqKarOeVentil(KARBUS_MSG_STRUCT* msg)
{
    Sensoroe* oe;
    oe = findSensorOe(msg->args[0]);
    if (oe != NULL) {
        OEBUS_PutTxMessage(msg->args[0], 1, REQ_OE_VENTIL);
        OEBUS_PutTxMessageArg(msg->args[1]);
        //KARBUS_PutTxMessage(msg->transmitter, 1, RES_KAR_OE_VENTIL);
        //KARBUS_PutTxMessageArg(msg->args[1]);
    } else {
        KARBUS_PutTxMessage(msg->transmitter,0,REQ_KAR_NONE);
    }
}

void handleReqKarOeSensorType(KARBUS_MSG_STRUCT* msg)
{
    Fieldsensor* fs;
    fs = findFieldsensor(msg->args[0], msg->args[1]);
    if (fs != NULL) {
        KARBUS_PutTxMessage(msg->transmitter,1,RES_KAR_OE_SENSOR_TYPE);
        KARBUS_PutTxMessageArg(fs->type);
    } else {
        KARBUS_PutTxMessage(msg->transmitter,0,REQ_KAR_NONE);
    }
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
        case RES_OE_FS_DATA:
            for ( i = 0; i < oer_connected; ++i) {
                if (oer[i]->addr == msg.transmitter) {
                    oer[i]->fieldsensor_connected = msg.len/5;
                    for (j=0; j<oer[i]->fieldsensor_connected; ++j) {
                        if (oer[i]->fieldsensors[j] == NULL)
                            oer[i]->fieldsensors[j] = malloc(sizeof(Fieldsensor));
                        
                        oer[i]->fieldsensors[j]->addr           = msg.args[j * 4 + 0];
                        oer[i]->fieldsensors[j]->type           = msg.args[j * 4 + 1];
                        oer[i]->fieldsensors[j]->status         = msg.args[j * 4 + 2];
                        oer[i]->fieldsensors[j]->values[FS_VALUE_HIGH]   = msg.args[j * 4 + 3];
                        oer[i]->fieldsensors[j]->values[FS_VALUE_LOW]    = msg.args[j * 4 + 4];
                    }
                    busy = 0;
                    break;
                }
            }
            busy = 0;
            break;
        case RES_OE_VENTIL:
            for ( i = 0; i < oer_connected; ++i) {
                if (oer[i]->addr == msg.transmitter) {
                    oer[i]->ventil = msg.args[0];
                    KARBUS_PutTxMessage(0x01, 1, RES_KAR_OE_VENTIL);
                    KARBUS_PutTxMessageArg(msg.args[0]);
                    break;
                }
            }
            break;
        default:
            break;
    }
    OEBUS_ClearRxMessage();
    state = 0;
}

void parseKARBUS()
{
    KARBUS_MSG_STRUCT msg;
    
    KARBUS_GetRxMessage(&msg);
    #if KARBUS_DEBUG_UART
        KARBUS_DebugMsg(&msg);
    #endif
    
    switch(msg.cmd) {
        case REQ_KAR_READY:
            KARBUS_PutTxMessage(msg.transmitter,0,RES_KAR_READY);
            break;
        case REQ_KAR_SENSOR_DATA:
            handleReqKarSensorData(&msg);
            break;
        case REQ_KAR_VENTIL:
            handleReqKarVentil(&msg);
            break;
        case REQ_KAR_PUMPE:
            handleReqKarPumpe(&msg);
            break;
        case REQ_KAR_OPRET:
            handleReqKarOpret(&msg);
            break;
        case REQ_KAR_OE_LIST:
            handleReqKarOeList(&msg);
            break;
        case REQ_KAR_OE_SENSOR_DATA:
            handleReqKarOeSensorData(&msg);
            break;
        case REQ_KAR_OE_VENTIL:
            handleReqKarOeVentil(&msg);
            break;
        case REQ_KAR_OE_SENSOR_TYPE:
            handleReqKarOeSensorType(&msg);
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
        printf("Addr: [ 0x%02X ]\t Type: [ 0x%02X ]\t Status: [ 0x%02X ]\t High: [ 0x%02X ]\t Low: [ 0x%02X ]\n\r", oe->fieldsensors[i]->addr, oe->fieldsensors[i]->type, oe->fieldsensors[i]->status, oe->fieldsensors[i]->values[0], oe->fieldsensors[i]->values[1]);
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

void Flow_DebugState()
{
    printf("Flow Liter:\t [ 0x%04X ]\n\r", flowLiter);
}

void debugConfig()
{
    printf("Kar config\n\r\n\r");
    printf("Kar Addr:\t [ 0x%02X ]\n\r", KARBUS_GetAddress());
    Indloeb_DebugState();
    Afloeb_DebugState();
    Flow_DebugState();
    printf("TX Int Mode:\t [ 0x%08X ]\n\r", (unsigned int)KARBUS_UART_GetTxInterruptMode());
    printf("TX Int Mode:\t [ 0x%08X ]\n\r", (unsigned int)KARBUS_UART_GetTxInterruptSource());
    printf("RX Int Mode:\t [ 0x%08X ]\n\r", (unsigned int)KARBUS_UART_GetRxInterruptMode());
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
            printf("s\tSend notify Rasp\n\r");
            printf("v\tSwitch polling\n\r");
            printf("\n\r");
            break;
        case 'v':
            pollEnabled = !pollEnabled;
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
        case 's':
            KARBUS_PutTxMessage(0x01,04,0x01);
            KARBUS_PutTxMessageArg(0x09);
            KARBUS_PutTxMessageArg(0x0D);
            KARBUS_PutTxMessageArg(0x0D);
            KARBUS_PutTxMessageArg(0x0D);
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
    pHProbe_Start();
    FlowSensor_Start();
    
    CyGlobalIntEnable;
    
    for(;;)
    {
        #if defined(DEBUG_UART)
        Debug_Communicate();
        #endif   

       if (OEBUS_ReadRxStatus() == OEBUS_MSG_READY)
            parseOEBUS();
        
        if (!busy && KARBUS_ReadRxStatus() == KARBUS_MSG_READY)
            parseKARBUS();
        
        if (state == 0) {
            if (oer[polling_nr] != 0) {
                if (pollEnabled)
                    pollSensoroe();
            }
        }
        
        pollKarSensor();
        CyDelay(1);
    }
}

/* [] END OF FILE */
